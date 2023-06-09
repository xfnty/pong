#include <game/states/gameplay.h>

#include <string.h>
#include <assert.h>
#include <math.h>

#include <raylib.h>

#include <system/strid.h>
#define SCOPE_NAME "gameplay"
#include <system/logging.h>
#include <system/defines.h>

#include <game/game.h>
#include <game/states/main_menu.h>


typedef struct gameplay_state_s {
    float greetings_start_time;
    unsigned int score[2];
    float left_pad;
    float right_pad;
    Vector2 ball_pos;
    Vector2 ball_dir;
    float ball_speed;
    bool is_paused;
    bool pad_is_ai[2];
} gameplay_state_t;

static void gameplay_restart(game_state_t* state, game_t* game, bool reset_score) {
    gameplay_state_t* gameplay = (gameplay_state_t*)state->data;

    *gameplay = (gameplay_state_t) {
        .greetings_start_time = 0,
        .score = { gameplay->score[0], gameplay->score[1] },
        .left_pad = 0,
        .right_pad = 0,
        .ball_pos = (Vector2){ game->canvas.texture.width/2, game->canvas.texture.height/2 },
        .ball_dir = (Vector2){ GetRandomValue(0, 1) * 2 - 1, GetRandomValue(-10, 10) / 10.0 },
        .ball_speed = 70.0f,
        .is_paused = false,
        .pad_is_ai = { gameplay->pad_is_ai[0], gameplay->pad_is_ai[1] }
    };

    if (reset_score)
        gameplay->score[0] = gameplay->score[1] = 0;
}

static void _gameplay_state_enter(game_state_t* state, game_t* game) {
    state->data = malloc(sizeof(gameplay_state_t));
    assert(state->data);
    memset(state->data, 0, sizeof(gameplay_state_t));

    gameplay_restart(state, game, true);
}

static void _gameplay_state_update(game_state_t* state, game_t* game, update_context_t ctx) {
    gameplay_state_t* gameplay = (gameplay_state_t*)state->data;

    const float width = game->canvas.texture.width;
    const float height = game->canvas.texture.height;
    const float pad_speed = 1.75f;
    const float pad_height = 8;
    const float ball_speed_inc = 7.5f;
    const float greetings_duration = 3;
    const unsigned int victory_score = 3;

    ClearBackground(BLACK);

    Rectangle left_pad_rect = (Rectangle) {
        .x = 4,
        .y = height / 2 + (height / 2 - pad_height / 2) * gameplay->left_pad - pad_height / 2,
        .width = 1,
        .height = pad_height
    };
    Rectangle left_pad_bounds_rect = (Rectangle) {
        left_pad_rect.x - 2,
        left_pad_rect.y - 1,
        left_pad_rect.width + 2,
        left_pad_rect.height + 2
    };
    Rectangle right_pad_rect = (Rectangle) {
        .x = width - 4,
        .y = height / 2 + (height / 2 - pad_height / 2) * gameplay->right_pad - pad_height / 2,
        .width = 1,
        .height = pad_height
    };
    Rectangle right_pad_bounds_rect = (Rectangle) {
        right_pad_rect.x,
        right_pad_rect.y - 1,
        right_pad_rect.width + 2,
        right_pad_rect.height + 2
    };
    Rectangle ball_rect = (Rectangle) {
        gameplay->ball_pos.x,
        gameplay->ball_pos.y - 1,
        2,
        2
    };

    const float dashed_line_segment_length = 3;
    const float dashed_line_segment_proportion = 0.67;
    for (float y = 0; y < height; y += dashed_line_segment_length) {
        DrawLine(width / 2, y, width / 2, y + dashed_line_segment_length * dashed_line_segment_proportion, DARKGRAY);
    }
    DrawRectangleRec(left_pad_rect, WHITE);
    DrawRectangleRec(right_pad_rect, WHITE);
    DrawRectangleRec(ball_rect, WHITE);

    DrawText(TextFormat("%d:%d", gameplay->score[0], gameplay->score[1]), game->canvas.texture.width / 2 - 8, 0, 8, WHITE);

    if (gameplay->greetings_start_time > 0) {
        // Congratulate the winner
        DrawRectangle(0, 0, width, height, (Color) { 0, 0, 0, 200 });
        DrawText((gameplay->score[0] == victory_score) ? "1st player wins!" : "2nd player wins!", width / 2 - 40, height / 2 - 8, 8, WHITE);
        if (gameplay->greetings_start_time + greetings_duration < GetTime() || GetKeyPressed() != 0) {
            gameplay->greetings_start_time = 0;
            gameplay_restart(state, game, true);
        }
    }
    else if (gameplay->is_paused) {
        DrawRectangle(0, 0, width, height, (Color) { 0, 0, 0, 200 });
        DrawText("Paused", width / 2 - 20, height / 2 - 8, 8, WHITE);
    }
    else {
        // Update pads
        float ai_pad_y = (gameplay->ball_pos.y / height) * 2 - 1;

        if (gameplay->pad_is_ai[0]) {
            gameplay->left_pad = ai_pad_y;
        }
        else {
            gameplay->left_pad += pad_speed * (IsKeyDown(KEY_S) - IsKeyDown(KEY_W)) * GetFrameTime();
        }

        if (gameplay->pad_is_ai[1]) {
            gameplay->right_pad = ai_pad_y;
        }
        else {
            gameplay->right_pad += pad_speed * (IsKeyDown(KEY_DOWN) - IsKeyDown(KEY_UP)) * GetFrameTime();
        }

        gameplay->left_pad = CONSTRAIN(gameplay->left_pad, -1, 1);
        gameplay->right_pad = CONSTRAIN(gameplay->right_pad, -1, 1);

        // Update ball
        if (gameplay->ball_pos.x < 0) {
            gameplay->score[1]++;
            LOG("2nd player scored a goal!");
            if (gameplay->score[1] >= victory_score) {
                gameplay->greetings_start_time = GetTime();
                LOG("Greetings!");
            }
            else {
                gameplay_restart(state, game, false);
            }
        }
        else if (gameplay->ball_pos.x > width) {
            gameplay->score[0]++;
            LOG("1st player scored a goal!");
            if (gameplay->score[0] >= victory_score) {
                gameplay->greetings_start_time = GetTime();
                LOG("Greetings!");
            }
            else {
                gameplay_restart(state, game, false);
            }
        }
        else if (CheckCollisionRecs(ball_rect, left_pad_bounds_rect) || CheckCollisionRecs(ball_rect, right_pad_bounds_rect)) {
            gameplay->ball_pos.x = CONSTRAIN(gameplay->ball_pos.x, 5, width - ball_rect.width - 5);
            gameplay->ball_dir.x *= -1;
            gameplay->ball_dir.y = GetRandomValue(-10, 10) / 20.0f ;
            gameplay->ball_speed += ball_speed_inc;
        }
        if (gameplay->ball_pos.y < 0 || gameplay->ball_pos.y > height) {
            gameplay->ball_pos.y = CONSTRAIN(gameplay->ball_pos.y, 1, height - 1);
            gameplay->ball_dir.y *= -1;
            gameplay->ball_speed += ball_speed_inc;
        }
        float vmag = sqrtf(gameplay->ball_dir.x*gameplay->ball_dir.x + gameplay->ball_dir.y*gameplay->ball_dir.y);
        if (vmag > 0) {
            gameplay->ball_dir.x /= vmag;
            gameplay->ball_dir.y /= vmag;
        }
        gameplay->ball_pos.x += gameplay->ball_dir.x * gameplay->ball_speed * GetFrameTime();
        gameplay->ball_pos.y += gameplay->ball_dir.y * gameplay->ball_speed * GetFrameTime();
    }

    if (IsKeyPressed(KEY_R)) {
        gameplay_restart(state, game, true);
        LOG("match was restarted");
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        game_switch_state(game, main_menu_state_create());
    }

    if (gameplay->greetings_start_time == 0 && IsKeyPressed(KEY_SPACE)) {
        gameplay->is_paused = !gameplay->is_paused;
    }

    if (IsKeyPressed(KEY_LEFT_BRACKET)) {
        gameplay->pad_is_ai[0] = !gameplay->pad_is_ai[0];
    }

    if (IsKeyPressed(KEY_RIGHT_BRACKET)) {
        gameplay->pad_is_ai[1] = !gameplay->pad_is_ai[1];
    }
}

static void _gameplay_state_exit(game_state_t* state, game_t* game) {
}

game_state_t gameplay_state_create() {
    return (game_state_t) {
        .name = strid_get_strid(SCOPE_NAME),
        .data = NULL,

        .on_enter = _gameplay_state_enter,
        .on_update = _gameplay_state_update,
        .on_exit = _gameplay_state_exit
    };
}
