import os, subprocess, shutil, uuid, hashlib
import common, setup, file_util

common.check_requests_package()
import requests

GEN_PATH = './gen/Windows'

SDL2_PATH = os.path.abspath("{}/SDL2".format(common.WINDOWS_LIB_PATH))
SDL2_IMAGE_PATH = os.path.abspath("{}/SDL2_image".format(common.WINDOWS_LIB_PATH))
SDL2_TTF_PATH = os.path.abspath("{}/SDL2_ttf".format(common.WINDOWS_LIB_PATH))

VISUAL_STUDIO_VERSION_TO_GENERATOR = {
    "14": "Visual Studio 14 2015",
    "15": "Visual Studio 15 2017",
    "16": "Visual Studio 16 2019",
}

def is_num(s):
  try:
    int(s)
    return True
  except:
    return False

def find_ms_build():
    # TODO make subprocess.exec use Popen (showOutput param could work then too)
    path = ""
    cmd = subprocess.Popen('"%ProgramFiles(x86)%/Microsoft Visual Studio/Installer/vswhere.exe" -nologo -latest -property installationPath', shell=True, stdout=subprocess.PIPE)
    for line in cmd.stdout:
        path = str(line).replace('b\'', '').replace('\\r\\n\'', '').replace('\\\\', '\\')

    return "{}/MSBuild/Current/Bin/MSBuild.exe".format(path)

def get_visual_studio_version():
    foundVersion = False
    version = 0
    cmd = subprocess.Popen('"./tools/vswhere.exe" -property installationVersion', shell=True, stdout=subprocess.PIPE)
    for line in cmd.stdout:
        version = str(line).split('.')[0].replace('b\'', '')
        if is_num(version):
            foundVersion = True
            break
    cmd.wait()

    if not foundVersion or cmd.returncode != 0:
        print()
        print("## OUI:")
        print("## Unable to find Visual Studio installation version")
        print("## Please install Visual Studio 2015 or higher with \"Desktop development with C++\" package")
        common.exit_error()

    return version

def build():

    if common.needs_setup():
        setup.setup()

    # Will show our "no Visual Studio" error instead of CMake's
    msbuild_location = find_ms_build()

    version = get_visual_studio_version()

    debug = False

    build_type = "Debug" if debug else "Release"

    print("\nGenerating project with CMake")
    common.exec([
        'cmake',
        '-G', VISUAL_STUDIO_VERSION_TO_GENERATOR[version],
        '-S', '.',
        '-B', GEN_PATH,
        "-DSDL2_PATH='{}'".format(SDL2_PATH),
        "-DSDL2_IMAGE_PATH='{}'".format(SDL2_IMAGE_PATH),
        "-DSDL2_TTF_PATH='{}'".format(SDL2_TTF_PATH),
        "-Dgtest_force_shared_crt=ON"
    ], "Could not generate project\nAre Visual Studio C++ tools and CMake 3.14+ installed?")

    print("\nBuilding project with MSBuild.exe")
    common.exec([
        '{}'.format(msbuild_location),
        '{}/ALL_BUILD.vcxproj'.format(GEN_PATH),
        '/p:Configuration={}'.format(build_type),
        '/p:Platform=x64'
    ], "Could not build project")

    outputFolder = "{}/windows".format(common.OUTPUT_FOLDER)

    print('\nCopying OUI headers')
    if os.path.isdir("{}/include".format(common.OUTPUT_FOLDER)):
        shutil.rmtree("{}/include".format(common.OUTPUT_FOLDER))
    shutil.copytree("include", "{}/include".format(common.OUTPUT_FOLDER))

    print("\nCopying OUI binaries")
    file_util.copyAllWithExt(
        path='{}/{}'.format(GEN_PATH, build_type),
        ext='dll',
        outputPath=outputFolder
    )
    file_util.copyAllWithExt(
        path='{}/{}'.format(GEN_PATH, build_type),
        ext='lib',
        outputPath=outputFolder
    )
    file_util.copyAllWithExt(
        path='{}/tests/{}'.format(GEN_PATH, build_type),
        ext='exe',
        outputPath=outputFolder
    )

    print("\nCopying SDL binaries")
    file_util.copyAllWithExt(
        path='{}/windows'.format(common.LIB_PATH),
        ext='dll',
        outputPath=outputFolder,
        excludeFolders=['x86']
    )

    print("\nFinished build")
    
if __name__ == "__main__":
    build()