#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <queue>
#include <vector>
#include <cstdlib>
#include "BomberMan.h"
using namespace std;
//########################
#ifdef PLAYER_A 
#undef PLAYER_A
#define PLAYER_A 1
#endif

#ifdef PLAYER_B
#undef PLAYER_B
#define PLAYER_B 0
#endif
//########################
//------------------------
//########################
//Base Define
#define MAXLEN 17
#define MAPLEN 15
#define MAXBOOM 20
#define MAXROOM 300
#define INF 0x7ffffff
#define TABLE 23
//########################
//------------------------
//########################
//Map ID
#define    BLANK      0
#define    WOOD       1
#define    STONE      2
#define    HOME       3
#define    BOMB       4
#define    ATTENTION  5
#define    EMHOME     6
#define    ENEMY      7
//########################
//------------------------
//########################
//Operator ID
#define 	NOP       0
#define 	UP        1
#define 	DOWN      2
#define 	LEFT      3
#define 	RIGHT     4
#define   PUT       5
//########################
//------------------------
//########################
//Data Struct Define 
typedef struct Edge
{
    int Dest;
    int Cost;
    Edge(int D,int C)
    {
        Dest=D;
        Cost=C;
    }
} Edge;
//########################
//------------------------
//########################
//Base Info 
int MyGroupID;
int Round;
int SourceMap[MAXLEN][MAXLEN];
int BombNum=0;
int BombX[MAXBOOM];
int BombY[MAXBOOM];
int BombHost[MAXBOOM];
int BombTime[MAXBOOM];
Player MyPlayer[PLAYER_NUM+2];
Player EmPlayer[PLAYER_NUM+2];
Home MyHome;
Home EmHome;
int Behave[PLAYER_NUM+2];
int Dir[4][2]={-1,0, //up
                1,0, //down
                0,-1, //left
                0,1//right
              };
FILE* Debug;
//########################
//------------------------
//########################
int Attack[MAXLEN][MAXLEN]={
          NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
          NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
          NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
          NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
          NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
          NOP,RIGHT,RIGHT,DOWN,DOWN,NOP,RIGHT,RIGHT,RIGHT,RIGHT,DOWN,NOP,RIGHT,RIGHT,RIGHT,DOWN,NOP,
          NOP,DOWN,NOP,RIGHT,RIGHT,RIGHT,UP,UP,NOP,RIGHT,RIGHT,RIGHT,RIGHT,UP,NOP,NOP,NOP,
          NOP,RIGHT,RIGHT,UP,NOP,NOP,UP,NOP,NOP,NOP,NOP,NOP,NOP,RIGHT,RIGHT,NOP,NOP,
          NOP,NOP,NOP,NOP,NOP,NOP,DOWN,NOP,NOP,NOP,NOP,RIGHT,RIGHT,DOWN,NOP,NOP,NOP,
          NOP,NOP,NOP,NOP,NOP,NOP,DOWN,NOP,DOWN,NOP,NOP,DOWN,NOP,DOWN,RIGHT,RIGHT,NOP,
          NOP,NOP,NOP,NOP,NOP,NOP,DOWN,DOWN,DOWN,DOWN,RIGHT,RIGHT,DOWN,DOWN,UP,NOP,NOP,
          NOP,NOP,NOP,NOP,NOP,NOP,RIGHT,RIGHT,RIGHT,RIGHT,UP,NOP,RIGHT,RIGHT,RIGHT,UP,NOP,
          NOP,NOP,NOP,NOP,NOP,NOP,UP,UP,UP,NOP,UP,RIGHT,UP,NOP,NOP,NOP,NOP,
          NOP,NOP,NOP,NOP,NOP,NOP,UP,NOP,UP,NOP,UP,NOP,NOP,NOP,NOP,NOP,NOP,
          NOP,NOP,NOP,NOP,NOP,NOP,UP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
};
int NormalA[MAXLEN][MAXLEN]={\
          //                                            
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,RIGHT,RIGHT,PUT,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,DOWN,NOP,NOP,NOP,RIGHT,RIGHT,RIGHT,DOWN,UP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,RIGHT,RIGHT,RIGHT,RIGHT,UP,NOP,NOP,RIGHT,RIGHT,UP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,UP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,UP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
    };
int BombA[MAXLEN][MAXLEN]={\
                                                      
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,DOWN,LEFT,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,DOWN,NOP,NOP,NOP,NOP,DOWN,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,DOWN,NOP,NOP,NOP,NOP,LEFT,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,DOWN,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
    };
int DefenceC[MAXLEN][MAXLEN]={\
                                                      
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,LEFT,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,UP,LEFT,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
    };
int DefenceCC[MAXLEN][MAXLEN]={\
                                                      
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,LEFT,LEFT,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,UP,UP,LEFT,LEFT,LEFT,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
    };
int DefenceB[MAXLEN][MAXLEN]={\
                                                      
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,DOWN,LEFT,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
    };
int DefenceBB[MAXLEN][MAXLEN]={\
                                                      
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,DOWN,LEFT,LEFT,LEFT,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,LEFT,LEFT,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
         NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
        NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,NOP,
    };
//########################
//------------------------
//########################
//Game Status Get Function
void Set();
void GetMap(const Game *game)
{
    Debug=fopen("Out.txt","a");
    //fprintf(Debug,"%d:",Round);
    //fprintf(Debug,"type:%d",game->map[10][15].type);
    for(int i=0;i<MAXLEN;i++)
        for(int j=0;j<MAXLEN;j++)
        {
            Object obj=game->map[i][j];
            int y=i;
            int x=j;
            if(MyGroupID==GROUP_GREEN)
                x=16-x;
            switch(obj.type)
            {
                case BLANK:
                    SourceMap[y][x]=BLANK;
                    break;
                case WOOD:
                    SourceMap[y][x]=WOOD;
                    break;
                case STONE:
                    SourceMap[y][x]=STONE;
                    break;
                case HOME:
                    SourceMap[y][x]=HOME;
                    if(obj.home.group==MyGroupID)
                        MyHome=obj.home;
                    else 
                        EmHome=obj.home;
                    break;
                case BOMB:
                    Bomb bome=obj.bomb;
                    BombNum++;
                    BombX[BombNum]=x;
                    BombY[BombNum]=y;
                    BombHost[BombNum]=bome.group;
                    BombTime[BombNum]=bome.TTL;
                    break;
            }
        }
      //fprintf(Debug,"Boom:%d\n",BombNum);
      //fclose(Debug);
}
void GetPlayer(const Game *game)
{
    for(int i=0;i<GROUP_NUM;i++)
    {
        Group group=game->group[i];
        if(group.group==MyGroupID)
        {
            for(int j=0;j<PLAYER_NUM;j++)
            {
                MyPlayer[j]=group.player[j];
                if(MyGroupID==GROUP_GREEN)
                  MyPlayer[j].pos.x=16-MyPlayer[j].pos.x;
            }
        }
        else 
        {
            for(int j=0;j<PLAYER_NUM;j++)
            {
                EmPlayer[j]=group.player[j];
                if(MyGroupID==GROUP_GREEN)
                  EmPlayer[j].pos.x=16-EmPlayer[j].pos.x;
            }
        }
    }
}
void GetBaseInfo(const Game* game)
{
    MyGroupID=game->grpid;
    Round=512-game->round;
}
void GetGame(const Game* game)
{
    GetBaseInfo(game);
    GetPlayer(game);
    GetMap(game);
    Set();
}
//########################
//------------------------
//########################
//Push Operator Function
int IsOverOp(int Op)
{
    if(MyGroupID==GROUP_GREEN)
    {
        if(Op==LEFT)
            Op=RIGHT;
        else if(Op==RIGHT)
            Op=LEFT;
    }
    return Op;
}
void SetBehaver(int ID,int Op)
{
    Op=IsOverOp(Op);
    Behave[ID]=Op;
}
void Push(Operator* op)
{
    for(int i=0;i<PLAYER_NUM;i++)
        op[i]=(Operator)Behave[i];
}
//########################
//------------------------
//########################
//Table Round
int Action[TABLE][3];
void Set()
{
    Action[0][PLAYER_A]=RIGHT;
    Action[0][PLAYER_B]=UP;
    Action[0][PLAYER_C]=DOWN;
    Action[1][PLAYER_A]=RIGHT;
    Action[1][PLAYER_B]=UP;
    Action[1][PLAYER_C]=DOWN;
    Action[2][PLAYER_A]=PUT;
    Action[2][PLAYER_B]=NOP;
    Action[2][PLAYER_C]=NOP;
    Action[3][PLAYER_A]=LEFT;
    Action[3][PLAYER_B]=DOWN;
    Action[3][PLAYER_C]=UP;
    Action[4][PLAYER_A]=LEFT;
    Action[4][PLAYER_B]=RIGHT;
    Action[4][PLAYER_C]=RIGHT;
    Action[5][PLAYER_A]=DOWN;
    Action[5][PLAYER_B]=RIGHT;
    Action[5][PLAYER_C]=RIGHT;
    Action[6][PLAYER_A]=NOP;
    Action[6][PLAYER_B]=RIGHT;
    Action[6][PLAYER_C]=RIGHT;
    Action[7][PLAYER_A]=NOP;
    Action[7][PLAYER_B]=NOP;
    Action[7][PLAYER_C]=NOP;
    Action[8][PLAYER_A]=NOP;
    Action[8][PLAYER_B]=DOWN;
    Action[8][PLAYER_C]=UP;
    Action[9][PLAYER_A]=NOP;
    Action[9][PLAYER_B]=NOP;
    Action[9][PLAYER_C]=NOP;
    Action[10][PLAYER_A]=DOWN;
    Action[10][PLAYER_B]=RIGHT;
    Action[10][PLAYER_C]=RIGHT;
    Action[11][PLAYER_A]=RIGHT;
    Action[11][PLAYER_B]=RIGHT;
    Action[11][PLAYER_C]=RIGHT;
    Action[12][PLAYER_A]=RIGHT;
    Action[12][PLAYER_B]=UP;
    Action[12][PLAYER_C]=DOWN;
    Action[13][PLAYER_A]=RIGHT;
    Action[13][PLAYER_B]=RIGHT;
    Action[13][PLAYER_C]=RIGHT;
    Action[14][PLAYER_A]=NOP;
    Action[14][PLAYER_B]=PUT;
    Action[14][PLAYER_C]=PUT;
    Action[15][PLAYER_A]=NOP;
    Action[15][PLAYER_B]=LEFT;
    Action[15][PLAYER_C]=LEFT;
    Action[16][PLAYER_A]=NOP;
    Action[16][PLAYER_B]=DOWN;
    Action[16][PLAYER_C]=UP;
    Action[17][PLAYER_A]=NOP;
    Action[17][PLAYER_B]=NOP;
    Action[17][PLAYER_C]=NOP;
    Action[18][PLAYER_A]=NOP;
    Action[18][PLAYER_B]=NOP;
    Action[18][PLAYER_C]=NOP;
    Action[19][PLAYER_A]=NOP;
    Action[19][PLAYER_B]=NOP;
    Action[19][PLAYER_C]=NOP;
    Action[20][PLAYER_A]=NOP;
    Action[20][PLAYER_B]=NOP;
    Action[20][PLAYER_C]=NOP;
    Action[21][PLAYER_A]=DOWN;
    Action[21][PLAYER_B]=UP;
    Action[21][PLAYER_C]=DOWN;
    Action[22][PLAYER_A]=RIGHT;
    Action[22][PLAYER_B]=RIGHT;
    Action[22][PLAYER_C]=RIGHT;
}
void AutoRun(int R)
{
    SetBehaver(PLAYER_A,Action[R][PLAYER_A]);
    SetBehaver(PLAYER_B,Action[R][PLAYER_B]);
    SetBehaver(PLAYER_C,Action[R][PLAYER_C]);
}
void Init(void)
{
    api_set_group_name("Toudsour");
}
//########################
Coord OptoPos(Coord Pos,int Op)
{
    if(Op>=1&&Op<=4)
    {
        Pos.x+=Dir[Op-1][1];
        Pos.y+=Dir[Op-1][0];
    }
    return Pos;
}
bool FindBomb(int X,int Y)
{
  for(int i=1;i<=BombNum;i++)
  {
      if(BombX[i]==X&&BombY[i]==Y&&BombHost[i]==MyGroupID)
        return true;
  }
  return false;
}
bool FindPlayer(int X,int Y)
{
    for(int i=0;i<PLAYER_NUM;i++)
    {
        if((EmPlayer[i].pos.x==X)&&(EmPlayer[i].pos.y==Y))
            return true;
    }
    return false;
}
bool warning()
{
    for(int i=0;i<PLAYER_NUM;i++)
    {
        if(EmPlayer[i].pos.x<=5)
            return true;
    }
    return false;
}
bool IsAtPos(Coord Pos,int X,int Y)
{
  if(Pos.x==X&&Pos.y==Y)
    return true;
  return false;
}
int GetAOp()
{
    Coord Pos=MyPlayer[PLAYER_A].pos;
    if(FindBomb(10,10)||FindBomb(15,10))
        return BombA[Pos.y][Pos.x];
    if(Pos.x==15&&Pos.y==10)
      return PUT;
    Coord Next=OptoPos(Pos,Attack[Pos.y][Pos.x]);
    fprintf(Debug, "Next Pos:X:%d,Y:%d",Next.x,Next.y);
    fprintf(Debug, "NextType:%d",SourceMap[Next.y][Next.x]);
    if(SourceMap[Next.y][Next.x]==WOOD)
        return PUT;
    return Attack[Pos.y][Pos.x];
}
int GetBOp()
{
    Coord Pos=MyPlayer[PLAYER_B].pos;
    if(Pos.x==1&&Pos.y==6)
        return NOP;
    if(FindBomb(7,5))
        return DefenceB[Pos.y][Pos.x];
    if(FindBomb(5,6))
        return DefenceBB[Pos.y][Pos.x];
    if(FindPlayer(9,5)&&IsAtPos(Pos,7,5))
        return PUT;
    if((FindPlayer(6,4)||FindPlayer(7,5)||FindPlayer(8,6))&&IsAtPos(Pos,6,6))
        return PUT;
    if(warning())
        return DefenceBB[Pos.y][Pos.x];
    return NOP;
}

int GetCop()
{
    Coord Pos=MyPlayer[PLAYER_C].pos; 
    if(Pos.x==1&&Pos.y==10)
        return NOP;
    if(FindBomb(7,11))
        return DefenceC[Pos.y][Pos.x];
    if(FindBomb(5,10))
        return DefenceCC[Pos.y][Pos.x];
    if(FindPlayer(9,11)&&IsAtPos(Pos,7,11))
        return PUT;
    if((FindPlayer(6,12)||FindPlayer(7,11)||FindPlayer(8,10))&&IsAtPos(Pos,6,10))
        return PUT;
    if(warning())
        return DefenceCC[Pos.y][Pos.x];
    return NOP;
}
void AI(const Game *game, Operator op[PLAYER_NUM])
{

    GetGame(game);
    //fprintf(Debug,"%d:",Round);
    if(Round<TABLE)
      AutoRun(Round);
    else
    {
      //fprintf(Debug,"Pos x:%d y:%d",MyPlayer[1].pos.x,MyPlayer[1].pos.y);
      SetBehaver(PLAYER_A,GetAOp());
      SetBehaver(PLAYER_B,GetBOp());
      SetBehaver(PLAYER_C,GetCop());
    }
    Push(op);
    //fprintf(Debug,"Out:%d\n",op[1]);
    //fclose(Debug);
}