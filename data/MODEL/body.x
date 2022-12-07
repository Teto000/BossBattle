xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 258;
 4.09520;71.37304;9.53834;,
 4.09520;71.37304;13.86398;,
 4.09520;63.34439;13.86398;,
 4.09520;63.34439;9.53834;,
 -4.09520;71.37304;13.86398;,
 -4.09520;71.37304;9.53834;,
 -4.09520;63.34439;9.53834;,
 -4.09520;63.34439;13.86398;,
 -4.09520;52.17641;26.53171;,
 4.09520;52.17641;26.53171;,
 -4.09520;52.14438;21.22749;,
 4.09520;52.14438;21.22749;,
 -4.09520;37.60363;15.15165;,
 4.09520;37.60363;15.15165;,
 -4.09520;45.14175;15.21064;,
 4.09520;45.14175;15.21064;,
 3.15642;71.37304;9.53834;,
 2.57159;63.34439;9.53834;,
 -4.09520;71.37304;10.91796;,
 -4.09520;63.34439;10.37522;,
 -6.21925;32.28972;-7.37628;,
 6.23316;32.28972;-7.37628;,
 6.23316;0.00689;-7.37628;,
 -6.21925;0.00689;-7.37628;,
 17.48230;37.28618;15.87524;,
 -17.48230;37.28618;15.87524;,
 -6.21925;32.28972;6.52028;,
 6.23316;32.28972;6.52028;,
 17.48230;37.28618;-16.73125;,
 -17.48230;37.28618;-16.73125;,
 17.48230;47.08718;15.87524;,
 -17.48230;47.08718;15.87524;,
 17.48230;47.08718;-16.73125;,
 -17.48230;47.08718;-16.73125;,
 -21.02121;45.93779;-3.51068;,
 -17.38846;45.93779;-3.51068;,
 -17.38846;38.81491;-3.51068;,
 -21.02121;38.81491;-3.51068;,
 -17.38846;45.93779;11.31676;,
 -17.38846;38.81491;11.31676;,
 -21.02121;45.93779;11.31676;,
 -21.02121;38.81491;11.31676;,
 17.34811;45.93779;-3.51068;,
 20.98086;45.93779;-3.51068;,
 20.98086;38.81491;-3.51068;,
 17.34811;38.81491;-3.51068;,
 20.98086;45.93779;11.31676;,
 20.98086;38.81491;11.31676;,
 17.34811;45.93779;11.31676;,
 17.34811;38.81491;11.31676;,
 -6.21925;32.28972;-2.47360;,
 -6.21925;0.00689;-0.75248;,
 6.23316;32.28972;6.13157;,
 6.23316;0.00689;6.52028;,
 6.23316;0.00689;4.77915;,
 5.55727;32.28972;6.52028;,
 3.20566;0.00689;6.52028;,
 6.23316;0.00689;0.44863;,
 6.23316;32.28972;-0.25368;,
 5.39683;30.08723;6.52028;,
 5.42998;32.28972;6.52028;,
 4.98965;0.00689;3.56529;,
 4.95962;0.00689;5.51157;,
 -6.21925;0.00689;6.52028;,
 -6.21925;3.20303;-0.92288;,
 -6.21925;17.16333;6.52028;,
 -6.21925;24.83081;6.52028;,
 -4.02316;5.69378;6.52028;,
 -3.37052;0.00689;6.52028;,
 -4.25123;0.00689;6.00145;,
 -6.21925;16.04998;5.92668;,
 -6.21925;24.83081;6.52028;,
 6.20356;31.24469;2.29267;,
 9.06290;31.24469;2.29267;,
 9.06290;24.43449;2.29267;,
 6.20356;24.43449;2.29267;,
 17.34648;31.24469;-1.93434;,
 14.65342;31.24469;-2.83391;,
 9.06290;31.24469;6.02572;,
 17.34648;24.43449;-1.93434;,
 9.06290;24.43449;6.02572;,
 14.65342;24.43449;-2.83391;,
 13.70888;31.24469;-14.08178;,
 12.50067;31.24469;-10.64688;,
 13.70888;24.43449;-14.08178;,
 12.50067;24.43449;-10.64688;,
 4.61957;27.84116;-13.21771;,
 6.20356;31.24469;5.90135;,
 6.20356;24.43449;5.90135;,
 5.14632;19.92681;5.90135;,
 5.14632;19.92681;2.29267;,
 5.14632;13.11660;2.29267;,
 5.14632;13.11660;5.90135;,
 16.28924;19.92681;-1.93434;,
 13.59619;19.92681;-2.83391;,
 8.00566;19.92681;2.29267;,
 8.00566;19.92681;6.02572;,
 16.28924;13.11660;-1.93434;,
 8.00566;13.11660;6.02572;,
 13.45986;14.61253;-2.34045;,
 8.00566;13.11660;2.29267;,
 12.65165;19.92681;-14.08178;,
 11.44344;19.92681;-10.64688;,
 12.65165;13.11660;-14.08178;,
 11.44344;13.31488;-10.64688;,
 3.91371;19.88999;-12.19778;,
 5.69166;19.92681;5.92507;,
 5.23046;13.11660;5.90501;,
 8.00566;19.92681;4.59428;,
 8.00566;13.11660;4.30898;,
 5.30913;19.92681;2.29267;,
 5.20662;13.11660;2.29267;,
 5.25337;19.92681;5.90601;,
 5.15093;13.11660;5.90155;,
 5.29395;19.92681;3.27602;,
 5.20597;13.11660;2.33474;,
 3.45748;9.55824;5.90135;,
 3.45748;9.55824;2.29267;,
 3.45748;2.74802;2.29267;,
 3.45748;2.74802;5.90135;,
 14.60040;2.74802;-1.93434;,
 14.60040;9.55824;-1.93434;,
 6.31681;9.55824;6.02572;,
 6.31681;2.74802;6.02572;,
 11.90734;2.74802;-2.83391;,
 6.31681;2.74802;2.29267;,
 11.90734;9.55824;-2.83391;,
 6.31681;9.55824;2.29267;,
 10.96280;9.55824;-14.08178;,
 7.15498;9.55824;-10.64688;,
 10.96280;2.74802;-14.08178;,
 9.45155;2.74802;-10.64688;,
 2.78615;11.49987;-12.18519;,
 4.87073;9.55824;5.96282;,
 4.40953;2.74802;5.94276;,
 6.31681;9.55824;5.13117;,
 8.63526;9.55824;3.79782;,
 5.15306;9.55824;2.29267;,
 5.05055;2.74802;2.29267;,
 4.99383;2.74802;5.96818;,
 5.09627;9.55824;5.97263;,
 5.09844;9.55824;5.83186;,
 4.99946;2.74802;5.60349;,
 6.31681;2.74802;4.84587;,
 5.02890;2.74802;3.69585;,
 -6.13569;24.43449;5.90135;,
 -6.13569;24.43449;2.29267;,
 -6.13569;31.24469;2.29267;,
 -6.13569;31.24469;5.90135;,
 -17.27862;24.43449;-1.93434;,
 -14.58556;24.43449;-2.83391;,
 -8.99503;24.43449;2.29267;,
 -8.99503;24.43449;6.02572;,
 -17.27862;31.24470;-1.93434;,
 -8.99503;31.24469;6.02572;,
 -14.58556;31.24470;-2.83391;,
 -8.99503;31.24469;2.29267;,
 -13.64102;24.43449;-14.08178;,
 -12.43281;24.43449;-10.64688;,
 -13.64102;31.24470;-14.08178;,
 -12.43281;31.24469;-10.64688;,
 -3.28960;27.83802;-13.21771;,
 -4.74171;13.11660;5.90135;,
 -4.74171;13.11660;2.29267;,
 -4.74171;19.92681;2.29267;,
 -4.74171;19.92681;5.90135;,
 -15.88464;13.11660;-1.93434;,
 -13.19157;13.11660;-2.83391;,
 -7.60105;13.11660;2.29267;,
 -7.60105;13.11660;6.02572;,
 -15.88464;19.92681;-1.93434;,
 -7.60105;19.92681;6.02572;,
 -13.19157;19.92681;-2.83391;,
 -7.60105;19.92681;2.29267;,
 -12.24703;13.11660;-14.08178;,
 -11.03882;13.11660;-10.64688;,
 -12.24703;19.92681;-14.08178;,
 -11.03882;18.00357;-10.64688;,
 -2.35891;19.64442;-12.19778;,
 -6.46405;13.11660;2.29267;,
 -6.89346;19.92681;2.29267;,
 -6.03065;13.11660;5.95742;,
 -6.45786;19.92681;5.97600;,
 -6.29566;17.34121;5.96894;,
 -5.84418;13.11660;5.94931;,
 -6.57197;19.92681;5.98096;,
 -6.04565;13.11660;5.83062;,
 -3.35247;2.74801;5.90135;,
 -3.35247;2.74801;2.29267;,
 -3.35247;9.55822;2.29267;,
 -3.35247;9.55822;5.90135;,
 -14.49540;2.74801;-1.93434;,
 -11.80234;2.74801;-2.83391;,
 -6.21181;2.74801;2.29267;,
 -6.21181;2.74801;6.02572;,
 -14.49540;9.55822;-1.93434;,
 -6.21181;9.55822;6.02572;,
 -10.85779;2.74801;-14.08178;,
 -9.64959;2.74801;-10.64688;,
 -10.85779;9.55822;-14.08178;,
 -9.64959;9.55822;-10.64688;,
 -11.80234;9.55822;-2.83391;,
 -1.43138;11.52293;-12.18519;,
 -5.81030;2.74801;2.29267;,
 -6.21181;9.11591;2.29267;,
 -6.21181;9.55822;2.29267;,
 -5.37314;2.74801;5.98924;,
 -5.80033;9.55822;6.00782;,
 -6.21181;9.55822;2.52845;,
 -6.24308;9.55824;2.26400;,
 -4.64061;2.74801;5.95738;,
 -5.36839;9.55822;5.98903;,
 -6.21181;2.74801;5.03179;,
 -5.43204;2.74801;5.49115;,
 -5.83506;9.55822;5.71412;,
 -6.21181;9.55822;5.49218;,
 6.74315;26.52564;11.86076;,
 3.42254;26.52564;17.58538;,
 3.42254;6.52564;17.58538;,
 6.74315;6.52564;11.86076;,
 -3.32063;26.52564;17.64279;,
 -3.32063;6.52564;17.64279;,
 -6.74320;26.52564;11.97556;,
 -6.74320;6.52564;11.97556;,
 -3.42257;26.52564;6.25093;,
 3.32062;26.52564;6.19353;,
 3.32062;6.52564;6.19353;,
 -3.42257;6.52564;6.25093;,
 -0.00001;32.35791;11.91816;,
 4.45737;32.35791;11.88365;,
 2.19500;32.35791;8.47617;,
 2.26237;32.35794;15.32564;,
 -2.19502;32.35794;15.36015;,
 -4.45742;32.35794;11.95267;,
 -0.00001;0.91947;11.91816;,
 2.10914;0.91947;8.28206;,
 4.28302;0.91947;11.88170;,
 2.17388;0.91947;15.51780;,
 -2.10916;0.91947;15.55427;,
 -4.28307;0.91947;11.95462;,
 -2.17391;0.91947;8.31853;,
 3.93541;26.52564;7.21154;,
 3.55985;6.52564;6.58967;,
 2.21069;32.27659;8.44434;,
 2.16085;32.35791;8.47643;,
 -2.26240;32.35794;8.51069;,
 2.92312;4.68621;6.87880;,
 0.18017;0.91947;8.29849;,
 -4.30082;26.52564;7.76500;,
 -3.71628;6.52564;6.75728;,
 -2.65617;32.35794;9.12815;,
 -1.33613;32.35792;9.90578;,
 -2.92678;4.29967;7.07189;,
 -1.33613;32.35794;9.90578;,
 -1.05659;32.35794;10.32681;,
 -0.83640;32.35792;10.20018;,
 0.56798;32.35791;11.02749;,
 -0.44765;0.91947;8.30383;;
 
 265;
 4;0,1,2,3;,
 4;4,1,0,5;,
 4;6,3,2,7;,
 4;8,9,1,4;,
 4;10,8,4,7;,
 4;11,10,7,2;,
 4;9,11,2,1;,
 4;12,13,9,8;,
 4;14,12,8,10;,
 4;15,14,10,11;,
 4;13,15,11,9;,
 4;13,12,14,15;,
 4;16,0,3,17;,
 4;16,17,6,5;,
 4;18,5,6,19;,
 4;18,19,7,4;,
 4;20,21,22,23;,
 4;24,25,26,27;,
 4;28,24,27,21;,
 4;29,28,21,20;,
 4;25,29,20,26;,
 4;30,31,25,24;,
 4;32,30,24,28;,
 4;33,32,28,29;,
 4;31,33,29,25;,
 4;31,30,32,33;,
 4;34,35,36,37;,
 4;35,38,39,36;,
 4;38,40,41,39;,
 4;40,34,37,41;,
 4;40,38,35,34;,
 4;37,36,39,41;,
 4;42,43,44,45;,
 4;43,46,47,44;,
 4;46,48,49,47;,
 4;48,42,45,49;,
 4;48,46,43,42;,
 4;45,44,47,49;,
 4;50,20,23,51;,
 4;52,27,53,54;,
 4;55,56,53,27;,
 3;54,53,56;,
 3;54,23,22;,
 4;57,22,21,58;,
 4;57,58,52,54;,
 3;59,55,60;,
 3;61,54,62;,
 3;61,63,23;,
 4;64,51,63,65;,
 3;66,63,67;,
 4;66,67,59,60;,
 3;66,60,26;,
 3;67,63,68;,
 4;67,68,56,59;,
 3;68,63,69;,
 4;68,69,61,62;,
 3;68,62,56;,
 3;70,65,71;,
 4;70,71,26,50;,
 3;70,50,64;,
 4;72,73,74,75;,
 4;76,77,73,78;,
 4;79,76,78,80;,
 4;81,79,80,74;,
 4;77,81,74,73;,
 4;82,83,77,76;,
 4;84,82,76,79;,
 4;85,84,79,81;,
 4;83,85,81,77;,
 3;86,83,82;,
 3;86,82,84;,
 3;86,84,85;,
 3;86,85,83;,
 4;73,72,87,78;,
 4;78,87,88,80;,
 4;87,72,75,88;,
 4;80,88,75,74;,
 4;89,90,91,92;,
 4;93,94,95,96;,
 4;97,93,96,98;,
 4;99,97,98,100;,
 4;94,99,100,95;,
 4;101,102,94,93;,
 4;103,101,93,97;,
 4;104,103,97,99;,
 4;102,104,99,94;,
 3;105,102,101;,
 3;105,101,103;,
 3;105,103,104;,
 3;105,104,102;,
 4;106,107,98,96;,
 3;106,96,108;,
 3;109,98,107;,
 4;110,95,100,111;,
 4;110,111,91,90;,
 4;112,89,92,113;,
 4;112,113,107,106;,
 3;110,90,114;,
 4;110,114,106,108;,
 3;110,108,95;,
 3;112,106,114;,
 4;112,114,90,89;,
 4;113,92,91,115;,
 4;113,115,109,107;,
 3;115,91,111;,
 4;115,111,100,109;,
 4;116,117,118,119;,
 4;120,121,122,123;,
 4;124,120,123,125;,
 4;126,124,125,127;,
 4;128,129,126,121;,
 4;130,128,121,120;,
 4;131,130,120,124;,
 4;129,131,124,126;,
 3;132,129,128;,
 3;132,128,130;,
 3;132,130,131;,
 3;132,131,129;,
 4;133,116,119,134;,
 3;133,117,116;,
 3;135,122,136;,
 4;135,136,121,126;,
 3;135,126,127;,
 4;137,127,125,138;,
 4;137,138,118,117;,
 4;139,123,122,140;,
 4;139,140,133,134;,
 3;141,133,140;,
 4;141,140,122,135;,
 4;141,135,127,137;,
 4;141,137,117,133;,
 3;139,134,142;,
 4;139,142,143,123;,
 3;144,143,142;,
 4;144,142,134,119;,
 3;144,119,118;,
 3;144,118,138;,
 4;144,138,125,143;,
 4;145,146,147,148;,
 4;149,150,151,152;,
 4;153,149,152,154;,
 4;155,153,154,156;,
 4;150,155,156,151;,
 4;157,158,150,149;,
 4;159,157,149,153;,
 4;160,159,153,155;,
 4;158,160,155,150;,
 3;161,158,157;,
 3;161,157,159;,
 3;161,159,160;,
 3;161,160,158;,
 4;147,156,154,148;,
 4;148,154,152,145;,
 4;156,147,146,151;,
 4;151,146,145,152;,
 4;162,163,164,165;,
 4;166,167,168,169;,
 4;170,166,169,171;,
 4;172,170,171,173;,
 4;167,172,173,168;,
 4;174,175,167,166;,
 4;176,174,166,170;,
 4;177,176,170,172;,
 4;175,177,172,167;,
 3;178,175,174;,
 3;178,174,176;,
 3;178,176,177;,
 3;178,177,175;,
 4;179,168,173,180;,
 4;179,180,164,163;,
 4;181,169,168,179;,
 4;180,173,171,182;,
 4;180,182,165,164;,
 3;183,181,184;,
 4;183,184,162,165;,
 3;183,165,182;,
 3;183,182,185;,
 4;183,185,171,169;,
 3;183,169,181;,
 3;184,181,186;,
 4;184,186,179,163;,
 3;184,163,162;,
 4;187,188,189,190;,
 4;191,192,193,194;,
 4;195,191,194,196;,
 4;197,198,192,191;,
 4;199,197,191,195;,
 4;200,199,195,201;,
 4;198,200,201,192;,
 3;202,198,197;,
 3;202,197,199;,
 3;202,199,200;,
 3;202,200,198;,
 3;203,193,204;,
 4;203,204,205,189;,
 3;203,189,188;,
 4;206,207,196,194;,
 3;208,189,205;,
 3;208,205,209;,
 4;208,209,201,195;,
 3;208,195,196;,
 3;209,205,204;,
 4;209,204,193,192;,
 3;209,192,201;,
 4;210,187,190,211;,
 4;210,211,207,206;,
 4;212,193,203,213;,
 4;212,213,206,194;,
 3;210,206,213;,
 4;210,213,203,188;,
 3;210,188,187;,
 3;214,208,215;,
 4;214,215,196,207;,
 3;214,207,211;,
 4;214,211,190,189;,
 3;214,189,208;,
 4;216,217,218,219;,
 4;217,220,221,218;,
 4;220,222,223,221;,
 4;224,225,226,227;,
 3;228,229,230;,
 4;229,231,217,216;,
 3;228,231,229;,
 4;231,232,220,217;,
 3;228,232,231;,
 4;232,233,222,220;,
 3;228,233,232;,
 3;234,235,236;,
 4;237,236,219,218;,
 3;234,236,237;,
 4;238,237,218,221;,
 3;234,237,238;,
 4;239,238,221,223;,
 3;234,238,239;,
 3;234,239,240;,
 3;234,240,235;,
 4;241,216,219,242;,
 4;241,242,226,225;,
 3;241,225,243;,
 4;241,243,230,229;,
 3;241,229,216;,
 3;244,230,243;,
 4;244,243,225,224;,
 3;244,224,245;,
 3;246,226,242;,
 4;246,242,219,236;,
 3;246,236,235;,
 3;246,235,247;,
 3;246,227,226;,
 4;248,224,227,249;,
 4;248,249,223,222;,
 4;250,245,224,248;,
 4;250,248,222,233;,
 3;251,245,250;,
 4;251,250,233,228;,
 3;249,227,252;,
 4;249,252,240,239;,
 3;249,239,223;,
 3;253,254,255;,
 4;253,255,244,245;,
 4;255,254,228,256;,
 4;255,256,230,244;,
 3;257,240,252;,
 4;257,252,227,246;,
 3;257,246,247;;
 
 MeshMaterialList {
  8;
  265;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  4,
  4,
  4,
  4,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  4,
  4,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  4,
  4,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.765600;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.044000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.385600;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.090980;0.000000;0.700000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.373600;0.373600;0.373600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.514400;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  214;
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.880562;0.473930;,
  0.000000;-0.880159;-0.474679;,
  0.000000;0.550780;0.834650;,
  0.000000;-0.549360;-0.835586;,
  -1.000000;0.000000;0.000000;,
  0.000000;-0.615479;0.788154;,
  0.000000;0.351175;-0.936310;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.651702;-0.758475;,
  0.000000;0.007825;-0.999969;,
  0.000000;0.000000;-1.000000;,
  0.213577;-0.944956;0.247876;,
  1.000000;0.000000;0.000000;,
  -0.213347;-0.945011;0.247865;,
  0.000000;-1.000000;-0.000000;,
  -0.213347;-0.945011;-0.247865;,
  0.213577;-0.944956;-0.247876;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  -0.362611;0.000000;-0.931941;,
  0.000000;1.000000;0.000000;,
  0.353217;0.000000;0.935541;,
  -0.043455;0.000000;0.999055;,
  0.000000;-1.000000;-0.000000;,
  0.692886;0.000000;0.721047;,
  -0.182638;0.981079;-0.064242;,
  -0.182789;-0.981048;-0.064295;,
  0.957970;0.000000;-0.286868;,
  -0.358365;0.925032;-0.126053;,
  -0.675863;0.000000;-0.737027;,
  -0.723339;0.000000;0.690493;,
  -0.964073;0.000000;0.265636;,
  -0.094638;0.000000;-0.995512;,
  -0.358649;-0.924909;-0.126153;,
  -0.310119;0.000000;0.950698;,
  -1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  -0.083289;-0.983068;-0.163219;,
  0.353217;0.000000;0.935541;,
  -0.235122;-0.967553;-0.092511;,
  0.692886;0.000000;0.721047;,
  -0.002280;0.999997;-0.000802;,
  -0.474535;-0.880049;-0.018202;,
  0.957970;0.000000;-0.286867;,
  -0.004560;0.999988;-0.001604;,
  0.000000;-1.000000;-0.000000;,
  -0.043455;-0.000000;0.999055;,
  0.000000;0.000000;-1.000000;,
  -0.043430;-0.000000;0.999057;,
  -0.083289;-0.983068;-0.163219;,
  -0.661530;-0.025529;-0.749484;,
  -0.353655;-0.013648;-0.935276;,
  -0.967747;0.024679;0.250714;,
  -0.210768;0.000000;-0.977536;,
  -0.626528;-0.759137;-0.176559;,
  -0.201735;0.000000;0.979440;,
  -0.043405;-0.000000;0.999058;,
  -1.000000;0.000000;0.000000;,
  -0.362610;0.000000;-0.931941;,
  0.000000;-1.000000;-0.000000;,
  0.353217;0.000000;0.935541;,
  -0.675863;0.000000;-0.737028;,
  0.692886;0.000000;0.721047;,
  0.147894;0.975319;0.163950;,
  -0.404691;-0.896952;-0.178052;,
  0.957970;0.000000;-0.286868;,
  0.288487;0.902496;0.319808;,
  0.000000;1.000000;0.000000;,
  -0.043457;0.000000;0.999055;,
  0.000000;0.000000;-1.000000;,
  -0.043457;0.000001;0.999055;,
  -0.688731;-0.144373;0.710497;,
  -0.902742;-0.140806;0.406486;,
  -0.225953;0.000000;-0.974138;,
  -0.725977;-0.609046;-0.319407;,
  -0.379514;-0.127981;0.916291;,
  -0.043454;-0.000000;0.999055;,
  0.000000;1.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000001;1.000000;0.000000;,
  -0.353217;0.000000;0.935541;,
  0.000002;1.000000;0.000000;,
  -0.692886;0.000000;0.721047;,
  0.161804;-0.985180;-0.056914;,
  0.161942;0.985155;-0.056960;,
  -0.957970;0.000000;-0.286868;,
  0.318813;-0.941160;-0.112141;,
  0.675863;0.000000;-0.737028;,
  0.362610;0.000000;-0.931941;,
  0.708954;0.000000;0.705254;,
  0.964073;0.000000;0.265636;,
  0.083184;0.000000;-0.996534;,
  0.319074;0.941061;-0.112230;,
  0.270677;0.000000;0.962670;,
  0.000000;1.000000;0.000000;,
  0.043455;0.000000;0.999055;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;0.000000;,
  -0.353217;-0.000000;0.935541;,
  0.225042;0.974166;0.018898;,
  -0.692886;0.000000;0.721047;,
  0.298336;-0.948674;-0.104938;,
  0.195419;0.936969;0.289656;,
  -0.957970;0.000000;-0.286869;,
  0.566048;-0.799967;-0.199105;,
  0.000000;0.000000;-1.000000;,
  0.043440;-0.000002;0.999056;,
  0.043455;0.000001;0.999056;,
  0.043449;-0.000000;0.999056;,
  -0.217520;0.000000;0.976056;,
  0.675863;0.000000;-0.737027;,
  0.362611;0.000000;-0.931941;,
  0.964073;0.000000;0.265636;,
  0.187165;0.000000;-0.982329;,
  -0.071951;0.859276;0.506427;,
  0.175891;0.000000;0.984410;,
  0.043451;0.000001;0.999056;,
  0.043458;0.000001;0.999055;,
  0.043458;-0.000000;0.999055;,
  0.043441;-0.000003;0.999056;,
  1.000000;0.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.362570;0.000000;-0.931957;,
  -0.353215;0.000000;0.935542;,
  -0.000012;1.000000;0.000013;,
  -0.692886;0.000000;0.721047;,
  0.374049;-0.918029;-0.131569;,
  -0.109856;0.993196;0.038641;,
  -0.957970;0.000000;-0.286869;,
  0.686776;-0.685553;-0.241568;,
  0.000000;0.000000;-1.000000;,
  0.362589;0.000001;-0.931949;,
  0.000061;1.000000;0.000004;,
  0.043457;0.000000;0.999055;,
  0.000183;1.000000;0.000013;,
  0.000000;1.000000;0.000000;,
  0.043455;0.000000;0.999055;,
  -0.000008;1.000000;0.000009;,
  0.964073;0.000000;0.265636;,
  0.197247;0.000000;-0.980354;,
  -0.218216;0.972877;0.076756;,
  0.183988;0.000000;0.982929;,
  0.362609;0.000002;-0.931942;,
  0.000195;1.000000;0.000000;,
  0.675841;0.000001;-0.737048;,
  0.675862;0.000002;-0.737028;,
  0.675863;0.000000;-0.737027;,
  0.043454;0.000000;0.999055;,
  0.478155;0.206033;-0.853767;,
  0.847821;0.166396;0.503499;,
  0.489726;0.200462;0.848518;,
  -0.474973;0.200570;0.856839;,
  -0.838651;0.166704;0.518531;,
  -0.228574;0.295660;-0.927544;,
  0.477229;-0.203887;-0.854800;,
  0.850945;-0.179594;0.493597;,
  0.491968;-0.203956;0.846386;,
  -0.477223;-0.203885;0.854804;,
  -0.842100;-0.179532;0.508563;,
  -0.231396;-0.291020;-0.928312;,
  0.000006;1.000000;-0.000005;,
  0.000002;1.000000;0.000001;,
  -0.000000;1.000000;-0.000004;,
  0.457770;0.388345;0.799771;,
  -0.443896;0.388497;0.807482;,
  -0.797733;0.328724;0.505532;,
  -0.454734;0.396182;-0.797657;,
  0.000000;-1.000000;-0.000000;,
  0.461260;-0.395632;0.794176;,
  -0.447449;-0.395506;0.802100;,
  -0.902644;-0.395469;-0.169819;,
  0.833462;0.205789;-0.512828;,
  0.842104;-0.179532;-0.508556;,
  0.000031;1.000000;-0.000021;,
  -0.007375;0.362956;-0.931777;,
  -0.007980;-0.347578;-0.937617;,
  0.440342;0.398657;-0.804470;,
  0.381179;-0.393953;-0.836363;,
  -0.847207;0.170892;-0.503027;,
  -0.844710;-0.215370;-0.489981;,
  -0.461260;-0.395634;-0.794175;,
  0.000013;1.000000;-0.000007;,
  -0.804391;0.336742;-0.489448;,
  0.000031;1.000000;-0.000019;,
  -0.007982;-0.347579;-0.937617;,
  0.000022;1.000000;-0.000013;,
  0.000007;1.000000;0.000003;,
  0.928329;0.371758;0.000769;,
  -0.000000;1.000000;-0.000007;,
  0.000003;1.000000;-0.000005;,
  0.000006;1.000000;-0.000001;,
  0.899517;-0.395424;-0.185764;,
  0.844698;0.162038;-0.510127;,
  0.842102;-0.179532;-0.508559;,
  0.431879;0.397861;-0.809436;,
  0.780654;0.349339;-0.518211;,
  0.447450;-0.395506;-0.802099;,
  -0.865010;-0.000000;-0.501754;,
  -0.850944;-0.179594;-0.493599;,
  -0.847689;0.167666;-0.503301;,
  -0.806282;0.330568;-0.490544;,
  0.000015;1.000000;-0.000004;,
  -0.461258;-0.395633;-0.794176;,
  0.000013;1.000000;-0.000005;;
  265;
  4;2,2,2,2;,
  4;3,3,0,0;,
  4;1,1,4,4;,
  4;5,5,3,3;,
  4;7,7,7,7;,
  4;6,6,4,4;,
  4;2,2,2,2;,
  4;8,8,8,8;,
  4;7,7,7,7;,
  4;9,9,11,11;,
  4;2,2,2,2;,
  4;12,12,9,9;,
  4;10,10,10,10;,
  4;10,10,10,10;,
  4;7,7,7,7;,
  4;7,7,7,7;,
  4;13,13,13,13;,
  4;14,16,16,14;,
  4;19,14,14,19;,
  4;18,19,19,18;,
  4;16,18,18,16;,
  4;20,20,20,20;,
  4;15,15,15,15;,
  4;13,13,13,13;,
  4;21,21,21,21;,
  4;22,22,22,22;,
  4;13,13,13,13;,
  4;15,15,15,15;,
  4;20,20,20,20;,
  4;21,21,21,21;,
  4;22,22,22,22;,
  4;17,17,17,17;,
  4;13,13,13,13;,
  4;15,15,15,15;,
  4;20,20,20,20;,
  4;21,21,21,21;,
  4;22,22,22,22;,
  4;17,17,17,17;,
  4;21,21,21,21;,
  4;15,15,15,15;,
  4;20,20,20,20;,
  3;17,23,17;,
  3;17,17,17;,
  4;15,15,15,15;,
  4;15,15,15,15;,
  3;20,20,20;,
  3;17,17,17;,
  3;17,17,17;,
  4;21,21,21,21;,
  3;20,20,20;,
  4;20,20,20,20;,
  3;20,20,20;,
  3;20,20,20;,
  4;20,20,20,20;,
  3;17,17,17;,
  4;17,17,17,17;,
  3;17,17,17;,
  3;21,21,21;,
  4;21,21,21,21;,
  3;21,21,21;,
  4;24,25,25,24;,
  4;26,26,26,26;,
  4;30,30,27,27;,
  4;29,29,29,29;,
  4;35,35,25,25;,
  4;31,31,26,26;,
  4;33,33,33,33;,
  4;32,32,29,29;,
  4;36,36,37,37;,
  3;34,31,31;,
  3;38,38,38;,
  3;39,32,32;,
  3;40,36,36;,
  4;26,26,26,26;,
  4;27,28,28,27;,
  4;41,41,41,41;,
  4;29,29,29,29;,
  4;42,42,42,42;,
  4;43,43,43,43;,
  4;47,47,45,45;,
  4;46,46,56,44;,
  4;57,57,58,58;,
  4;48,48,43,43;,
  4;50,50,50,50;,
  4;49,49,46,46;,
  4;59,59,59,59;,
  3;51,48,48;,
  3;60,60,60;,
  3;61,49,49;,
  3;62,62,62;,
  4;53,53,45,45;,
  3;43,43,43;,
  3;52,56,52;,
  4;54,58,58,54;,
  4;54,54,54,54;,
  4;55,63,63,55;,
  4;55,55,53,53;,
  3;43,43,43;,
  4;43,43,43,43;,
  3;43,43,43;,
  3;43,43,43;,
  4;43,43,43,43;,
  4;52,52,52,52;,
  4;52,52,52,52;,
  3;52,52,52;,
  4;52,52,44,52;,
  4;64,64,64,64;,
  4;69,69,67,67;,
  4;66,66,66,66;,
  4;68,68,65,65;,
  4;70,70,74,74;,
  4;72,72,72,72;,
  4;71,71,66,66;,
  4;78,78,79,79;,
  3;73,70,70;,
  3;80,80,80;,
  3;81,71,71;,
  3;82,78,78;,
  4;75,83,83,75;,
  3;74,74,84;,
  3;74,74,74;,
  4;74,74,74,74;,
  3;74,74,74;,
  4;76,65,65,76;,
  4;76,76,76,76;,
  4;77,67,67,77;,
  4;77,77,75,75;,
  3;74,74,74;,
  4;74,74,74,74;,
  4;74,74,74,74;,
  4;74,74,74,74;,
  3;66,66,66;,
  4;66,66,66,66;,
  3;66,66,66;,
  4;66,66,66,66;,
  3;66,66,66;,
  3;66,66,66;,
  4;66,66,66,66;,
  4;85,85,85,85;,
  4;86,86,86,86;,
  4;90,90,88,88;,
  4;89,89,87,87;,
  4;95,95,96,96;,
  4;91,91,86,86;,
  4;93,93,93,93;,
  4;92,92,89,89;,
  4;97,97,98,98;,
  3;94,91,91;,
  3;99,99,99;,
  3;100,92,92;,
  3;101,97,97;,
  4;102,87,87,102;,
  4;103,88,88,103;,
  4;96,104,104,96;,
  4;86,86,86,86;,
  4;105,105,105,105;,
  4;106,106,106,106;,
  4;110,110,119,108;,
  4;109,109,107,107;,
  4;120,120,121,121;,
  4;111,111,106,106;,
  4;113,113,113,113;,
  4;112,112,109,109;,
  4;122,122,122,122;,
  3;114,111,111;,
  3;123,123,123;,
  3;124,112,112;,
  3;125,125,125;,
  4;115,121,121,115;,
  4;115,115,115,115;,
  4;106,106,106,106;,
  4;107,107,107,107;,
  4;107,107,107,107;,
  3;118,126,117;,
  4;118,117,127,128;,
  3;118,128,129;,
  3;118,129,116;,
  4;118,116,108,119;,
  3;118,119,126;,
  3;106,106,106;,
  4;106,106,106,106;,
  3;106,106,106;,
  4;130,130,130,130;,
  4;131,131,131,131;,
  4;135,135,133,133;,
  4;136,136,131,131;,
  4;138,138,138,138;,
  4;137,137,147,134;,
  4;148,148,148,148;,
  3;139,136,136;,
  3;149,149,149;,
  3;150,137,137;,
  3;151,151,151;,
  3;140,152,141;,
  4;140,141,132,140;,
  3;140,140,140;,
  4;143,143,133,133;,
  3;142,145,153;,
  3;142,153,144;,
  4;142,144,134,147;,
  3;142,147,145;,
  3;154,132,141;,
  4;154,141,152,155;,
  3;154,155,156;,
  4;146,157,157,146;,
  4;146,146,143,143;,
  4;131,131,131,131;,
  4;131,131,131,131;,
  3;131,131,131;,
  4;131,131,131,131;,
  3;131,131,131;,
  3;145,142,145;,
  4;145,145,145,145;,
  3;145,145,145;,
  4;145,145,145,145;,
  3;145,145,142;,
  4;159,160,166,165;,
  4;160,161,167,166;,
  4;161,162,168,167;,
  4;163,158,164,169;,
  3;170,172,171;,
  4;197,173,160,159;,
  3;170,198,172;,
  4;173,174,161,160;,
  3;170,199,198;,
  4;174,175,162,161;,
  3;170,200,199;,
  3;177,177,177;,
  4;178,201,165,166;,
  3;177,177,177;,
  4;179,178,166,167;,
  3;177,177,177;,
  4;180,179,167,168;,
  3;177,177,177;,
  3;177,177,177;,
  3;177,177,177;,
  4;181,202,203,182;,
  4;181,182,164,158;,
  3;181,158,186;,
  4;181,186,204,205;,
  3;181,197,202;,
  3;184,204,186;,
  4;184,186,158,163;,
  3;184,163,176;,
  3;187,164,182;,
  4;187,182,203,201;,
  3;187,201,206;,
  3;187,206,185;,
  3;187,169,164;,
  4;188,207,207,189;,
  4;188,189,208,209;,
  4;192,176,163,188;,
  4;192,188,209,210;,
  3;196,211,196;,
  4;196,196,200,170;,
  3;189,169,190;,
  4;189,190,212,180;,
  3;189,180,208;,
  3;193,183,195;,
  4;193,195,213,211;,
  4;195,183,170,191;,
  4;195,191,171,213;,
  3;194,212,190;,
  4;194,190,169,187;,
  3;194,187,185;;
 }
}
