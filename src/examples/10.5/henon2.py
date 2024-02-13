#!/usr/bin/python3

import mpmath
import ex2

def main():
  res,count = ex2.sample_main(1)
  emax= mpmath.mpf(-123456789)
  emin= mpmath.mpf(123456789)
  for i in range(count):
    a=mpmath.fadd(mpmath.fmul(res[2][i], res[2][i]),mpmath.fmul(res[3][i], res[3][i])); # xp**2+yp**2
    b=mpmath.fmul(res[0][i], res[0][i])    # x**2
    c=mpmath.fmul(res[1][i], res[1][i])    # y**2
    d=mpmath.fadd(b,c)    # x**2+y**2
    e=mpmath.fadd(d, mpmath.fmul(2.0,mpmath.fmul(b, res[1][i])))    # x**2+y**2 + 2 x**2 y
    f=mpmath.fdiv(mpmath.fmul(2.0,mpmath.fmul(c, res[1][i])), 3.0) # 2./3. * y**3
    g=mpmath.fsub(e,f)    # x**2+y**2 +x**2 y - 2/3 y**3
    h=mpmath.fmul(0.5,mpmath.fadd(a, g)) # 0.5*(xp**2+yp**2) + 0.5*(x**2+y**2+2*x**2*y -2./3. * y**3)
    if(h > emax):
      emax = h
    if(h < emin):
      emin = h
    print("EnergyMax", emax)
    print("EnergyMin", emin)
    print("EnergyRange:", mpmath.fsub(emax,emin))

if __name__ == "__main__":
  main()
