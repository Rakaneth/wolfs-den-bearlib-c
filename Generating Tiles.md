# Basics:

3x3 sections (2^n):
    7 0 1
    6 * 2
    5 4 3

| Wall Direction | Wall Mask |
| -------------- | --------- |
| N              | 0x83      |
| E              | 0x07      |
| S              | 0x38      |
| W              | 0xE0      |

tileset(by integer index):
* 0
* 1, 4, 16, 64
    # . #
    # . #
    # # #
* 5, 20, 80, 65
    # . #
    # . .
    # # #
* 7, 28, 112, 193
    # . .
    # . .
    # # #
* 17, 68
    # . #
    # . #
    # . #
* 21, 84, 81, 69
    # . #
    # . .
    # . #
* 23, 92, 113, 197
    # . .
    # . .
    # . #
* 29, 116, 209, 71
    # . #
    # . .
    # . .
* 31, 124, 241, 199
    # . .
    # . .
    # . .
* 85
    # . #
    . . .
    # . #
* 87, 93, 117, 213
    # . .
    . . .
    # . #
* 95, 125, 245, 215
    # . .
    . . .
    # . .
* 119, 221
    # . .
    . . .
    . . #
* 127, 253, 247, 223
    # . .
    . . .
    . . .
* 255
    . . .
    . . .
    . . .

rotations: (4x index) % 256 rotates clockwise


