final int PIXEL = 64;
final int LAYER_NUM = 9;
final int LIGHT_THRESH = 160;
float shadowScaleX = 0.07;
float shadowScaleY = 0.05;
float[][][] leaves = new float[LAYER_NUM][PIXEL][PIXEL];
float[] shadowNoiseBase = new float[LAYER_NUM];
float[] windBase = new float[LAYER_NUM];
//float[] windSpeed = new float[LAYER_NUM];
float[][] windDirection = new float[LAYER_NUM][2];
float[][] result = new float[PIXEL][PIXEL];
float trembling =0;
float tremblingSpeed =0.02;
float windSpeed = 0.004;
float windNoiseZ = 0.5;
float smallWind = 0;
float smallWindSpeed = 0.03;
float smallwindRange = 0.001;
float smallWindNoiseZ = 0.3;

int PIXEL_W = 640/PIXEL;
void setup(){
  size(640,640);
  initLeaves();
  noStroke();
}

void draw(){
  fill(0);
  genRandomLeaves();
  rect(0,0,width,height);
  for(int x=0;x<PIXEL;x++){
    for(int y=0;y<PIXEL;y++){
      result[x][y] = 0;
      for(int l=0;l<LAYER_NUM;l++){
        result[x][y] += leaves[l][x][y];
        constrain(result[x][y],0,255);
      }
      fill(result[x][y],200);
      //fill(leaves[0][x][y]);
      //ellipse(x*PIXEL_W+PIXEL_W/2,y*PIXEL_W+PIXEL_W/2,result[x][y]/10,result[x][y]/10 );
      rect(x*PIXEL_W,y*PIXEL_W,PIXEL_W*1,PIXEL_W*1);
    }
  }
}

void initLeaves(){
  for(int l=0;l<LAYER_NUM;l++){
    shadowNoiseBase[l]=l*10;//random(0,5);
    windBase[l]=random(0,0.2);
    windDirection[l][0]=3;//random(-2,2);
    windDirection[l][1]=random(0,2);
  }
}

void genRandomLeaves(){
  float tempNoise = 0;
  float windRangeX=0, windRangeY=0;
  float finalNoiseX=0, finalNoiseY=0;
  for(int l=0;l<LAYER_NUM;l++){
    for(int x=0;x<PIXEL;x++){
      for(int y=0;y<PIXEL;y++){
        windRangeX=noise(windBase[l])*(windDirection[l][0]+(y*0.01));
        windRangeY=noise(windBase[l])*(windDirection[l][1]+(x*0.01));
        finalNoiseX=shadowNoiseBase[l]+x*shadowScaleX+windRangeX+(noise(smallWind)-0)*smallwindRange;
        finalNoiseY=shadowNoiseBase[l]+y*shadowScaleY+windRangeY+(noise(smallWind)-0)*smallwindRange;


        tempNoise = noise(finalNoiseX,
                          finalNoiseY,
                          (windRangeX+windRangeY)*windNoiseZ+noise(smallWind)*smallWindNoiseZ);
        leaves[l][x][y] = tempNoise*255;


        if(leaves[l][x][y]>LIGHT_THRESH) leaves[l][x][y]=map(leaves[l][x][y],LIGHT_THRESH,LIGHT_THRESH+60,0,255);
        else leaves[l][x][y]=0;
      }
    }
    windBase[l]+=windSpeed;
  }
  trembling+=tremblingSpeed;
  smallWind+=smallWindSpeed;
}
