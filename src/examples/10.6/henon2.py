#!/usr/bin/python3
import ex4
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

def main():
    res,count = ex4.sample_main(1)
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    plt.plot(res[0],res[1],res[2],linestyle=":")
    zeros=np.zeros(count);
    # x y xp yp x u1 v1 y u2 v2 xp u3 v3 yp u4 v4 t
    u = np.array([res[5],res[6],zeros])
    v = np.array([res[8],res[9],zeros])
    ulength=np.linalg.norm(u)
    vlength=np.linalg.norm(v)
    log_ulength = np.log(ulength)
    log_vlength = np.log(vlength)
    ax.quiver(res[0],res[1],res[2], res[5],res[6],zeros,
    pivot='tail',length=0.4*log_ulength/ulength,arrow_length_ratio=0.3,color="green")
    ax.quiver(res[0],res[1],res[2], res[8],res[9],zeros,
    pivot='tail',length=0.4*log_vlength/vlength,arrow_length_ratio=0.3,color="red")
  
    plt.show()
if __name__ == "__main__":
   main()