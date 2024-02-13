 extern MY_FLOAT G, m[3];

qx0' = px0;
qy0' = py0;
qz0' = pz0;
qx1' = px1;
qy1' = py1;
qz1' = pz1;
qx2' = px2;
qy2' = py2;
qz2' = pz2;

px0' =
+G*m[1]*(qx0-qx1)/((qx0-qx1)**2+(qy0-qy1)**2+(qz0-qz1)**2)**1.5
+G*m[2]*(qx0-qx2)/((qx0-qx2)**2+(qy0-qy2)**2+(qz0-qz2)**2)**1.5
;
py0' =
+G*m[1]*(qy0-qy1)/((qx0-qx1)**2+(qy0-qy1)**2+(qz0-qz1)**2)**1.5
+G*m[2]*(qy0-qy2)/((qx0-qx2)**2+(qy0-qy2)**2+(qz0-qz2)**2)**1.5
;
pz0' =
+G*m[1]*(qz0-qz1)/((qx0-qx1)**2+(qy0-qy1)**2+(qz0-qz1)**2)**1.5
+G*m[2]*(qz0-qz2)/((qx0-qx2)**2+(qy0-qy2)**2+(qz0-qz2)**2)**1.5
;
px1' =
+G*m[0]*(qx1-qx0)/((qx1-qx0)**2+(qy1-qy0)**2+(qz1-qz0)**2)**1.5
+G*m[2]*(qx1-qx2)/((qx1-qx2)**2+(qy1-qy2)**2+(qz1-qz2)**2)**1.5
;
py1' =
+G*m[0]*(qy1-qy0)/((qx1-qx0)**2+(qy1-qy0)**2+(qz1-qz0)**2)**1.5
+G*m[2]*(qy1-qy2)/((qx1-qx2)**2+(qy1-qy2)**2+(qz1-qz2)**2)**1.5
;
pz1' =
+G*m[0]*(qz1-qz0)/((qx1-qx0)**2+(qy1-qy0)**2+(qz1-qz0)**2)**1.5
+G*m[2]*(qz1-qz2)/((qx1-qx2)**2+(qy1-qy2)**2+(qz1-qz2)**2)**1.5
;
px2' =
+G*m[0]*(qx2-qx0)/((qx2-qx0)**2+(qy2-qy0)**2+(qz2-qz0)**2)**1.5
+G*m[1]*(qx2-qx1)/((qx2-qx1)**2+(qy2-qy1)**2+(qz2-qz1)**2)**1.5
;
py2' =
+G*m[0]*(qy2-qy0)/((qx2-qx0)**2+(qy2-qy0)**2+(qz2-qz0)**2)**1.5
+G*m[1]*(qy2-qy1)/((qx2-qx1)**2+(qy2-qy1)**2+(qz2-qz1)**2)**1.5
;
pz2' =
+G*m[0]*(qz2-qz0)/((qx2-qx0)**2+(qy2-qy0)**2+(qz2-qz0)**2)**1.5
+G*m[1]*(qz2-qz1)/((qx2-qx1)**2+(qy2-qy1)**2+(qz2-qz1)**2)**1.5
;
