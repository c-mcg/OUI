{
    'variables': {
        'SDL2_WIN_PATH': 'lib/windows'
    },
    'includes': [],
    'target_defaults': {},
    'targets': [
      {
        'target_name': 'OUI',
        'type': '<(library)',
        # 'dependencies': [],
        # 'defines': [],
        'include_dirs': [
          'include/',
        ],
        'sources': [
          'source/**/*.cpp',
        ],

        'conditions': [
            # ['OS=="mac"', {
            #     'libraries': ['lib/OUI-engine/bin/mac/libOUI.so']
            # }],
            ['OS=="win"', {
                'libraries': [
                    '<(PRODUCT_DIR)/../../<(SDL2_WIN_PATH)/SDL2/lib/x64/SDL2',
                    '<(PRODUCT_DIR)/../../<(SDL2_WIN_PATH)/SDL2_image/lib/x64/SDL2_image',
                    '<(PRODUCT_DIR)/../../<(SDL2_WIN_PATH)/SDL2_ttf/lib/x64/SDL2_ttf',
                ],
                'include_dirs': [
                    '<(SDL2_WIN_PATH)/SDL2/include',
                    '<(SDL2_WIN_PATH)/SDL2_image/include',
                    '<(SDL2_WIN_PATH)/SDL2_ttf/include',
                ],
                'copies': [
                    {
                        'destination': '<(PRODUCT_DIR)',
                        'files': [
                            '<(SDL2_WIN_PATH)/SDL2/lib/x64/SDL2.dll',
                            '<(SDL2_WIN_PATH)/SDL2_image/lib/x64/SDL2_image.dll',
                            '<(SDL2_WIN_PATH)/SDL2_image/lib/x64/libjpeg-9.dll',
                            '<(SDL2_WIN_PATH)/SDL2_image/lib/x64/libpng16-16.dll',
                            '<(SDL2_WIN_PATH)/SDL2_image/lib/x64/libtiff-5.dll',
                            '<(SDL2_WIN_PATH)/SDL2_image/lib/x64/libwebp-7.dll',
                            '<(SDL2_WIN_PATH)/SDL2_image/lib/x64/zlib1.dll',
                            '<(SDL2_WIN_PATH)/SDL2_ttf/lib/x64/SDL2_ttf.dll',
                            '<(SDL2_WIN_PATH)/SDL2_ttf/lib/x64/libfreetype-6.dll',
                        ]
                    },
                ],
            }],
        ],
      },
    ],
  }