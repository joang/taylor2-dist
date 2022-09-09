extern MY_FLOAT e1, e2, coef[10], freq[10]; /* declare some external vars */
diff(x,t) = e1 * y;
diff(y,t) = -x + e2*sum( coef[i] * sin( freq[i] * t), i = 0, 9);

/* 
taylor -name perturbation -header -o taylor.h perturbation.eq
taylor -step -jet -o perturbation.c -name perturbation perturbation.eq
*/
