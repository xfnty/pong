#include <game/states/gameplay.h>

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
    unsigned int score[2];
    float left_pad;
    float right_pad;
    Vector2 ball_pos;
    Vector2 ball_dir;
} gameplay_state_t;

static void gameplay_restart(game_state_t* state, game_t* game, bool reset_score) {
    gameplay_state_t* gameplay = (gameplay_state_t*)state->data;

    *gameplay = (gameplay_state_t) {
        .score = { gameplay->score[0], gameplay->score[1] },
        .left_pad = 0,
        .right_pad = 0,
        .ball_pos = (Vector2){ game->canvas.texture.width/2, game->canvas.texture.height/2 },
        .ball_dir = (Vector2){ GetRandomValue(0, 1) * 2 - 1, GetRandomValue(-10, 10) / 10.0 }
    };

    if (reset_score)
        gameplay->score[0] = gameplay->score[1] = 0;
}

static void _gameplay_state_enter(game_state_t* state, game_t* game) {
    state->data = malloc(sizeof(gameplay_state_t));
    assert(state->data);

    gameplay_restart(state, game, true);
}

static void _gameplay_state_update(game_state_t* state, game_t* game, update_context_t ctx) {
    gameplay_state_t* gameplay = (gameplay_state_t*)state->data;

    const float width = game->canvas.texture.width;
    const float height = game->canvas.texture.height;
    const float pad_speed = 1.75f;
    const float pad_height = 8;
    const float ball_speed = 60.0f;

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
    DrawRectangleRec(left_pad_rect, WHITE);
    DrawRectangleRec(right_pad_rect, WHITE);
    DrawRectangleRec(ball_rect, WHITE);

    DrawText(TextFormat("%d:%d", gameplay->score[0], gameplay->score[1]), game->canvas.texture.width / 2 - 8, 0, 8, WHITE);

    // Update pads
    gameplay->left_pad += pad_speed * (IsKeyDown(KEY_S) - IsKeyDown(KEY_W)) * GetFrameTime();
    gameplay->right_pad += pad_speed * (IsKeyDown(KEY_DOWN) - IsKeyDown(KEY_UP)) * GetFrameTime();

    gameplay->left_pad = CONSTRAIN(gameplay->left_pad, -1, 1);
    gameplay->right_pad = CONSTRAIN(gameplay->right_pad, -1, 1);

    // Update ball
    if (gameplay->ball_pos.x < 0) {
        gameplay->score[1]++;
        gameplay_restart(state, game, false);
        LOG("right player scored a goal!");
    }
    else if (gameplay->ball_pos.x > width) {
        gameplay->score[0]++;
        gameplay_restart(state, game, false);
        LOG("left player scored a goal!");
    }
    else if (CheckCollisionRecs(ball_rect, left_pad_bounds_rect) || CheckCollisionRecs(ball_rect, right_pad_bounds_rect)) {
        // gameplay->ball_pos.x = CONSTRAIN(gameplay->ball_pos.x, 5, height - 5);
        gameplay->ball_dir.x *= -1;
        gameplay->ball_dir.y = GetRandomValue(-10, 10) / 7.5 ;
    }
    if (gameplay->ball_pos.y < 0 || gameplay->ball_pos.y > height) {
        gameplay->ball_pos.y = CONSTRAIN(gameplay->ball_pos.y, 1, height - 1);
        gameplay->ball_dir.y *= -1;
    }
    float vmag = sqrtf(gameplay->ball_dir.x*gameplay->ball_dir.x + gameplay->ball_dir.y*gameplay->ball_dir.y);
    if (vmag > 0) {
        gameplay->ball_dir.x /= vmag;
        gameplay->ball_dir.y /= vmag;
    }
    gameplay->ball_pos.x += gameplay->ball_dir.x * ball_speed * GetFrameTime();
    gameplay->ball_pos.y += gameplay->ball_dir.y * ball_speed * GetFrameTime();

    if (IsKeyPressed(KEY_R)) {
        gameplay_restart(state, game, true);
        LOG("match was restarted");
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        game_switch_state(game, main_menu_state_create());
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
