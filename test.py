frame: list[float] = [
    0, 5, 0,
    5, 2, 5,
    0, 5, 0,
]

frame_4x3: list[float] = [
    0, 5, 4, 1,
    5, 3, 3, 0,
    1, 4, 2, 0,
]

def aa(image, width, height, scale):
    AA_WIDTH: int = width*scale-width*2
    AA_HEIGHT: int = height*scale-height*2
    BUF_SIZE: int = AA_WIDTH * AA_HEIGHT
    aa_frame: list[float] = [0 for _ in range(BUF_SIZE)]

    aa_buf_len: int = AA_WIDTH*height
    aa_buf: list[float] = [0 for _ in range(aa_buf_len)]

    for h in range(height):
        # Create an interpolated row
        aa_row: list[float] = [0 for _ in range(AA_WIDTH)]

        for w in range(width):
            # Get the first two temperature points
            t1: float
            t2: float
            if w+1 != width:
                t1 = image[h*width + w]
                t2 = image[h*width + w+1]
            else:
                break

            # Get the delta and calculate the step
            dt: float = t2-t1
            step: float = dt / (scale - 1)

            # Add the following pixels from the frame to an AA row buffer
            for i in range(scale):
                idx: int = w*(scale-1) + i
                val: float = t1 + (step * i)
                print(w, scale-1, i, idx, val)
                # aa_row[idx] = val
        print(h, aa_row)

        # Cache the interpolated row to be used in Y-axis interpolation later
        for i in range(AA_WIDTH):
            idx: int = (h*AA_WIDTH) + i
            # print(idx, aa_row[i], end=', ')
            aa_buf[idx] = aa_row[i]
        # print()

    # Do the same thing for the Y-axis of the frame
    for w in range(AA_WIDTH):
        # Create an interpolated column
        aa_col: list[float] = [0 for _ in range(AA_HEIGHT)]

        for h in range(height):
            # Get the first two temperature points
            t1: float
            t2: float
            # print(w, h, w*height + h, end=', ')
            if h+1 != height:
                t1 = aa_buf[h*AA_WIDTH+w]
                t2 = aa_buf[h*AA_WIDTH+w + AA_WIDTH]
            else:
                continue
            # print(f"{t1}, {t2}")

            # Get the delta and calculate the step
            dt: float = t2-t1
            step: float = dt / (scale - 1)

            # Add the following pixels from the frame to an AA row buffer
            for i in range(scale):
                idx: int = h*(scale-1) + i
                val: float = t1 + (step * i)
                aa_col[idx] = val
        # print(w, aa_col)

        # Cache the interpolated row to be used in Y-axis interpolation later
        for i in range(AA_HEIGHT):
            idx: int = (i*AA_WIDTH) + w
            # print(idx, aa_col[i], end=', ')
            aa_frame[idx] = aa_col[i]
        # print()
    return aa_frame, AA_WIDTH, AA_HEIGHT

def printImage(image, width, height):
    print('-'*30)
    for h in range(height):
        for w in range(width):
            print(image[(h*width)+w], end=', ')
        print()

# printImage(frame_4x3, 4, 3)
aa_frame, aa_width, aa_height = aa(frame_4x3, 4, 3, 5)
printImage(aa_frame, aa_width, aa_height)