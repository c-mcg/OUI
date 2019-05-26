import os, shutil
import common, setup, file_util

GEN_PATH = './gen/Linux'

def build():
    if common.needs_setup():
        setup.setup()

    print("\nGenerating project with CMake")
    common.exec([
        'cmake',
        '-S', '.',
        '-B', GEN_PATH,
        "-Dgtest_force_shared_crt=ON"
    ], "Could not generate project")

    os.chdir('gen/Linux')
    common.exec(['make'], "Could not build OUI engine")
    os.chdir('../..')

    outputFolder = "{}/linux".format(common.OUTPUT_FOLDER)

    print('\nCopying OUI headers')
    if os.path.isdir("{}/include".format(common.OUTPUT_FOLDER)):
        shutil.rmtree("{}/include".format(common.OUTPUT_FOLDER))
    shutil.copytree("include", "{}/include".format(common.OUTPUT_FOLDER))

    print("\nCopying OUI binaries")
    file_util.copyAllWithExt(
        path='{}'.format(GEN_PATH),
        ext='so',
        outputPath=outputFolder
    )

if __name__ == "__main__":
    build()