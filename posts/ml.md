Ridge vs lasso

Ридж/ridge
==========

<svg viewBox="-3 -6 7 9" width="15em">
<line x1="-10" x2="10" y1="0" y2="0" />
<line x1="0" x2="0" y1="-10" y2="10" />
<circle cx="0" cy="0" r="0.05" class="filled" />
<circle cx="0" cy="0">
<animate
attributeName="r"
values="
0.93;
2.46;
0.93;
"
dur="5s"
repeatCount="indefinite" />
</circle>
<circle cx="1" cy="-3" r="0.05" style="fill-opacity:1" />
<circle cx="1" cy="-3">
<animate
attributeName="r"
values="
2.24;
0.71;
2.24;
"
dur="5s"
repeatCount="indefinite">
</circle>
<line class="dashed" x1="-2" y1="6" x2="2" y2="-6" />
</svg>

Точка пересечения сохраняет угол по отношению к началу координат.
(В случае эллипсов это не так, но пока проигнорируем это.)

Лассо/lasso
===========

<svg viewBox="-3 -6 7 9" width="15em">
<line x1="-10" x2="10" y1="0" y2="0" />
<line x1="0" x2="0" y1="-10" y2="10" />
<circle cx="0" cy="0" r="0.05" class="filled" />
<polygon>
<animate
attributeName="points"
values="
1,0 0,1 -1,0, 0,-1 ;
2,0 0,2 -2,0, 0,-2 ;
3,0 0,3 -3,0, 0,-3 ;
2,0 0,2 -2,0, 0,-2 ;
1,0 0,1 -1,0, 0,-1 ;
"
dur="5s"
repeatCount="indefinite" />
</polygon>
<circle cx="1" cy="-3" r="0.05" style="fill-opacity:1" />
<circle cx="1" cy="-3">
<animate
attributeName="r"
values="
2.24;
1.41;
0.71;
1.41;
2.24;
"
dur="5s"
repeatCount="indefinite" />
</circle>
<line class="dashed" x1="-10" y1="8" x2="8" y2="-10"  />
</svg>

Точка пересечения перемещается к углу при уменьшении ромба, находясь на перпендикуляре к его стороне.
При дальнейшем уменьшении ромба точка остается на углу.
Так лассо зануляет признаки.
