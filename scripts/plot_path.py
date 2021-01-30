#! usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl

def main():

    x, y = np.loadtxt('../path.txt', delimiter=' ', unpack=True)
    arr = np.loadtxt('../grid.txt')
    #np.flipud(arr)

    plt.figure(1)

    plt.plot(x, y, c='r', label='path')
    plt.hlines(y=0.0, xmin=0.0, xmax=10.0, colors='k', linestyles='solid')
    plt.hlines(y=5.0, xmin=5.0, xmax=10.0, colors='k', linestyles='solid')
    plt.hlines(y=10.0, xmin=0.0, xmax=5.0, colors='k', linestyles='solid')
    plt.vlines(x=0.0, ymin=0.0, ymax=10.0, colors='k', linestyles='solid')
    plt.vlines(x=5.0, ymin=5.0, ymax=10.0, colors='k', linestyles='solid')
    plt.vlines(x=10.0, ymin=0.0, ymax=5.0, colors='k', linestyles='solid')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.title('Position')
    plt.axis('equal')
    plt.legend()

    #plt.show()

    plt.figure(2)
    # define the colors
    cmap = mpl.colors.ListedColormap(['w', 'k'])

    # create a normalize object the describes the limits of
    # each color
    bounds = [0., 0.5, 1.]
    norm = mpl.colors.BoundaryNorm(bounds, cmap.N)

    # plot it
    plt.imshow(arr, interpolation='none', cmap=cmap, norm=norm, origin='lower')
    plt.title('Area Swept')
    plt.axis('equal')
    plt.show()


if __name__ == "__main__":
    main()
