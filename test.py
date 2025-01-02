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

def ss(image, width, height, scale):
    if scale < width and scale < height: scale = width if width < height else height
    SS_WIDTH:  int = width*(scale-2)  # -2 because the first and last value of a row/column are not interpolated
    SS_HEIGHT: int = height*(scale-2) # -2 because the first and last value of a row/column are not interpolated
    BUF_SIZE:  int = SS_WIDTH * SS_HEIGHT
    ss_frame:  list[float] = [0 for _ in range(BUF_SIZE)]
    # print(f"{SS_WIDTH=}, {SS_HEIGHT=}")

    ss_buf_len: int = SS_WIDTH*height
    ss_buf: list[float] = [0 for _ in range(ss_buf_len)]

    for h in range(height):
        # Create an interpolated row
        ss_row: list[float] = [0 for _ in range(SS_WIDTH)]

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

            # Add the following pixels from the frame to an ss row buffer
            # print(len(ss_row))
            for i in range(scale):
                idx: int = w*(scale-1) + i # TODO: Mess with `scale-1` offset
                if idx >= len(ss_row): break
                val: float = t1 + (step * i)
                # print(w, scale-1, i, idx, val)
                ss_row[idx] = val
        # print(h, ss_row)

        # Cache the interpolated row to be used in Y-axis interpolation later
        for i in range(SS_WIDTH):
            idx: int = (h*SS_WIDTH) + i
            # print(idx, ss_row[i], end=', ')
            ss_buf[idx] = ss_row[i]
        # print()

    # Do the same thing for the Y-axis of the frame
    for w in range(SS_WIDTH):
        # Create an interpolated column
        ss_col: list[float] = [0 for _ in range(SS_HEIGHT)]

        for h in range(height):
            # Get the first two temperature points
            t1: float
            t2: float
            # print(w, h, w*height + h, end=', ')
            if h+1 != height:
                t1 = ss_buf[h*SS_WIDTH+w]
                t2 = ss_buf[(h+1)*SS_WIDTH+w]
            else:
                break
            # print(f"{t1}, {t2}")

            # Get the delta and calculate the step
            dt: float = t2-t1
            step: float = dt / (scale - 1)

            # Add the following pixels from the frame to an ss row buffer
            # print(len(ss_col))
            for i in range(scale):
                idx: int = h*(scale-1) + i # TODO: Mess with `scale-1` offset
                if idx >= len(ss_col): break
                val: float = t1 + (step * i)
                # print(h, scale, i, idx, val)
                ss_col[idx] = val
        # print(w, ss_col)

        # Cache the interpolated row to be used in Y-axis interpolation later
        for i in range(SS_HEIGHT):
            idx: int = (i*SS_WIDTH) + w
            # print(idx, ss_col[i], end=', ')
            ss_frame[idx] = ss_col[i]
        # print()
    return ss_frame, SS_WIDTH, SS_HEIGHT


def printImage(image, width, height):
    print(f"WIDTH: {width}, HEIGHT: {height}")
    print('-'*30)
    for h in range(height):
        for w in range(width):
            print(f"{image[(h*width)+w]:0.3f}", end=', ')
        print()
    print('-'*30)

printImage(frame_4x3, 4, 3)
ss_frame, ss_width, ss_height = ss(frame_4x3, 4, 3, 5)
printImage(ss_frame, ss_width, ss_height)