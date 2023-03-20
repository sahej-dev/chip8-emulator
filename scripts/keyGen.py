# sdl to keyscancode

# keys = list("1234567890QWERTYUIOPASDFGHJKLZXCVBNM")
# keys.append("SPACE")
# for key in keys:
    # print(f"case SDL_SCANCODE_{key}: return KeyScanCode::k{key};")


# keyscancode to chip8key
keys = list("0123456789ABCDEF")
for key in keys:
    print(f"case emuGL::KeyScanCode::k{key}: chip8Key = 0x{key}; break;")