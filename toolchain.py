#!/usr/bin/env python3

import os
import re
import sys
import shutil
import subprocess as sp
import multiprocessing as mp
import argparse


class static_property:
    def __init__(self, getter, *args, **kwargs):
        self.args = args
        self.kwargs = kwargs
        self.getter = getter
    def __get__(self, obj, objtype):
        return self.getter(*self.args, **self.kwargs)
    @staticmethod
    def __call__(cls, *args, **kwargs):
        return static_property(cls, *args, **kwargs)


class Settings:
    @static_property
    def PROJECT_DIR():
        return os.path.realpath(os.path.dirname(__file__))

    @static_property
    def CMAKE_DIR():
        return os.path.join(Settings.PROJECT_DIR, '.cmake')

    @static_property
    def IS_CONFIGURED():
        return os.path.exists(Settings.CMAKE_DIR)

    @static_property
    def BUILDS_DIR():
        return os.path.join(Settings.PROJECT_DIR, 'build')

    @static_property
    def ASSETS_DIR():
        return os.path.join(Settings.PROJECT_DIR, 'assets')

    @static_property
    def SANDBOX_DIR():
        return os.path.join(Settings.BUILDS_DIR, 'sandbox')

    @staticmethod
    def get_cmake_dir_for(build_type):
        return os.path.join(Settings.CMAKE_DIR, build_type)

    @staticmethod
    def get_build_dir_for(build_type):
        return os.path.join(Settings.BUILDS_DIR, build_type)

    @staticmethod
    def get_assets_dir_for(build_type):
        return os.path.join(Settings.get_build_dir_for(build_type), 'assets')

    @staticmethod
    def get_exe_dir_for(build_type):
        return os.path.join(
            Settings.BUILDS_DIR,
            build_type
        )

    @staticmethod
    def get_project_name_for(build_type):
        if Settings.IS_CONFIGURED:
            CMakeCache = open(
                os.path.join(
                    Settings.get_cmake_dir_for(build_type),
                    'CMakeCache.txt'
                )
            ).read()
            return re.search(
                r'(?<=CMAKE_PROJECT_NAME:STATIC=)(.*)',
                CMakeCache
            )[0]
        return None

    @staticmethod
    def get_exe_path_for(build_type):
        return os.path.join(
            Settings.get_exe_dir_for(build_type),
            Settings.get_project_name_for(build_type)
        )



def _run_command(
        title: str, 
        cmd: str, 
        cwd: str=Settings.PROJECT_DIR,
        print_exit_code=False,
        verbose=False) -> bool:
    try:
        print(f'----- {title} -----')
        if verbose:
            print(f'> {cmd}')
        r = sp.check_call(cmd, cwd=cwd, shell=True)
        if print_exit_code:
            print(f'----- Exited with code {r} -----')
    except Exception as e:
        print(str(e))
        return False

    return True


def copy_assets(build_type):
    out_dir = Settings.get_assets_dir_for(build_type)
    shutil.rmtree(out_dir, ignore_errors=True)
    shutil.copytree(Settings.ASSETS_DIR, out_dir, dirs_exist_ok=True)


def configure(build_type):
    return _run_command(
        'Configuring',
        f'cmake -DCMAKE_BUILD_TYPE={build_type.capitalize()}' +
        f' -B "{Settings.get_cmake_dir_for(build_type)}"' +
        f' -S "{Settings.PROJECT_DIR}"',
    )


def build(build_type):
    copy_assets(build_type)
    return _run_command(
        'Building',
        f'cmake --build {Settings.get_cmake_dir_for(build_type)} -j{mp.cpu_count()}',
    )


def run(build_type, args=[]):
    exe_dir = Settings.get_exe_dir_for(build_type)
    exe_path = Settings.get_exe_path_for(build_type)

    return _run_command(
        'Running',
        f'{exe_path} ' + ' '.join(args),
        cwd=exe_dir,
        print_exit_code=True,
    )


def debug(build_type, args=[]):
    gdb_path = shutil.which('gdb') or 'gdb'
    exe_dir = Settings.get_exe_dir_for(build_type)
    exe_path = Settings.get_exe_path_for(build_type)

    return _run_command(
        'Debugging',
        f'{gdb_path} -q --return-child-result'
        + f' {exe_path} ' + ('--args ' + ' '.join(args) if args else ''),
        cwd=exe_dir
    )


def run_sandbox(args=[]):
    exe_dir = Settings.SANDBOX_DIR
    exe_path = os.path.join(exe_dir, 'sandbox')

    return _run_command(
        'Running sandbox',
        f'{exe_path} ' + ' '.join(args),
        cwd=exe_dir,
        print_exit_code=True,
    )


def clean(build_type):
    shutil.rmtree(Settings.get_cmake_dir_for(build_type), ignore_errors=True)
    shutil.rmtree(Settings.SANDBOX_DIR, ignore_errors=True)
    shutil.rmtree(Settings.BUILDS_DIR, ignore_errors=True)


def main():
    arg_parser = argparse.ArgumentParser(
        description='Configure, build and run this project using CMake.' + 
            ' This script is here to replace ".sh" and ".bat" scripts.'
    )
    arg_parser.add_argument(
        '-c', '--config',
        dest='configure',
        action='store_true',
        help='Configure only.'
    )
    arg_parser.add_argument(
        '-b', '--build',
        dest='build',
        action='store_true',
        help='Build only.'
    )
    run_group = arg_parser.add_mutually_exclusive_group()
    run_group.add_argument(
        '-r', '--run',
        dest='run',
        action='store_true',
        help='Run compiled executable.'
    )
    run_group.add_argument(
        '-d', '--debug',
        dest='debug',
        action='store_true',
        help='Run project with GDB.'
    )
    run_group.add_argument(
        '-s', '--sandbox',
        dest='sandbox',
        action='store_true',
        help='Run sandbox project.'
    )
    arg_parser.add_argument(
        '--clean',
        dest='clean',
        action='store_true',
        help='Clean up CMake and build folders from the beginning.'
    )
    arg_parser.add_argument(
        '--release',
        dest='build_type',
        action='store_const',
        default='debug',
        const='release',
        help='Set build type to "release" (default is "debug").'
    )
    arg_parser.add_argument(
        '-a', '--args',
        nargs=argparse.REMAINDER,
        default=[],
        help='Specify command line arguments for the program to be run with.'
    )
    args = arg_parser.parse_args()
    build_type = args.build_type

    if args.clean:
        clean(build_type)

    if args.configure:
        configure(build_type)

    if args.build:
        if not Settings.IS_CONFIGURED:
            print(
                'Project was not configured.' +
                f' Run "{os.path.basename(__file__) } -c" to configure CMake.'
            )
            sys.exit(-1)

        if not build(build_type):
            sys.exit(-1)

    if args.run:
        if not run(build_type, args.args):
            sys.exit(-1)
    elif args.debug:
        if not debug(build_type, args.args):
            sys.exit(-1)
    elif args.sandbox:
        if not run_sandbox(args.args):
            sys.exit(-1)


if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print()
