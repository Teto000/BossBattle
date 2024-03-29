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
 36;
 13.18078;0.00000;-29.22155;,
 13.18078;-20.66274;-20.66276;,
 -14.05842;-20.94630;-20.65977;,
 -14.05842;-0.28354;-29.21854;,
 13.18077;-29.22153;-0.00000;,
 -14.05842;-29.50507;0.00299;,
 13.18078;-20.66274;20.66275;,
 -14.05842;-20.94630;20.66574;,
 13.18078;0.00000;29.22153;,
 -14.05842;-0.28354;29.22452;,
 13.18078;20.66274;20.66273;,
 -14.05842;20.37923;20.66572;,
 13.18078;29.22153;0.00000;,
 -14.05842;28.93795;0.00298;,
 13.18901;20.37921;-20.65977;,
 -14.05842;20.37921;-20.65977;,
 13.18078;0.00000;0.00000;,
 -14.05842;-0.28354;0.00299;,
 13.83368;0.00000;0.00000;,
 13.83368;-11.87832;-11.87833;,
 13.83368;0.00000;-16.79849;,
 13.83368;-16.79848;-0.00000;,
 13.83368;-11.87832;11.87832;,
 13.83368;0.00000;16.79848;,
 13.83368;11.87833;11.87832;,
 13.83368;16.79851;0.00000;,
 13.83368;11.87833;-11.87833;,
 -14.68222;-0.28354;-0.08276;,
 -14.68222;-0.28354;-16.79550;,
 -14.68222;-12.16186;-11.87534;,
 -14.68222;-17.08204;0.00299;,
 -14.68222;-12.16186;11.88131;,
 -14.68222;-0.28354;16.80147;,
 -14.68222;11.59478;11.79557;,
 -14.68222;16.51498;-0.08276;,
 -14.68222;11.59477;-11.87534;;
 
 48;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,0,3,15;,
 3;16,1,0;,
 3;16,4,1;,
 3;16,6,4;,
 3;16,8,6;,
 3;16,10,8;,
 3;16,12,10;,
 3;16,14,12;,
 3;16,0,14;,
 3;17,3,2;,
 3;17,2,5;,
 3;17,5,7;,
 3;17,7,9;,
 3;17,9,11;,
 3;17,11,13;,
 3;17,13,15;,
 3;17,15,3;,
 3;18,19,20;,
 3;18,21,19;,
 3;18,22,21;,
 3;18,23,22;,
 3;18,24,23;,
 3;18,25,24;,
 3;18,26,25;,
 3;18,20,26;,
 3;27,28,29;,
 3;27,29,30;,
 3;27,30,31;,
 3;27,31,32;,
 3;27,32,33;,
 3;27,33,34;,
 3;27,34,35;,
 3;27,35,28;,
 4;35,26,20,28;,
 4;28,20,19,29;,
 4;29,19,21,30;,
 4;30,21,22,31;,
 4;31,22,23,32;,
 4;32,23,24,33;,
 4;33,24,25,34;,
 4;34,25,26,35;;
 
 MeshMaterialList {
  8;
  48;
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
   0.800000;0.091200;0.000000;0.700000;;
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
  27;
  1.000000;-0.000051;0.000050;,
  0.000996;0.001259;-0.999999;,
  0.007283;-0.707082;-0.707094;,
  0.010409;-0.999946;-0.000000;,
  0.007438;-0.707082;0.707093;,
  0.000110;0.000000;1.000000;,
  -0.007283;0.707083;0.707093;,
  -0.007793;0.999965;-0.002935;,
  -0.003701;0.705920;-0.708282;,
  -1.000000;0.000000;0.000000;,
  1.000000;-0.000000;0.000000;,
  1.000000;-0.000202;-0.000000;,
  1.000000;-0.000000;-0.000000;,
  1.000000;-0.000000;-0.000000;,
  1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000199;,
  1.000000;-0.000202;0.000199;,
  1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  -0.006787;0.706631;-0.707550;,
  -0.000105;-0.000000;-1.000000;,
  0.006957;-0.707084;-0.707095;,
  0.009943;-0.999951;0.000000;,
  0.007105;-0.707084;0.707094;,
  -0.000663;0.001535;0.999999;,
  -0.008341;0.708161;0.706002;,
  -0.010252;0.999947;-0.000645;;
  48;
  4;1,2,2,1;,
  4;2,3,3,2;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,8,8,7;,
  4;8,1,1,8;,
  3;0,10,11;,
  3;0,12,10;,
  3;0,13,12;,
  3;0,14,13;,
  3;0,14,14;,
  3;0,15,14;,
  3;0,16,15;,
  3;0,11,16;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  3;18,18,18;,
  4;19,19,20,20;,
  4;20,20,21,21;,
  4;21,21,22,22;,
  4;22,22,23,23;,
  4;23,23,24,24;,
  4;24,24,25,25;,
  4;25,25,26,26;,
  4;26,26,19,19;;
 }
}
