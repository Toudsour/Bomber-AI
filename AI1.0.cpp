//#include"Table.h"
#include<queue>
#include<stack>
#include"GetStatus.h"
//########################
//Path Data
vector<Edge>  Link[MAXROOM];
int Path[MAXROOM];
int Short[MAXROOM];
int BombMap[MAXLEN][MAXLEN];
//########################
//------------------------
//########################
//Run to Dest Point
void GetSourceMap(int Source[][MAXLEN],int Dest[][MAXLEN])
{
    for(int i=0;i<MAXLEN;i++)
        for(int j=0;j<MAXLEN;j++)
            Dest[i][j]=Source[i][j];
}
void Change(int X,int Y,int T)
{
    if(BombMap[Y][X]<0)
        return;
    if(BombMap[Y][X]>=T)
        return;
    BombMap[Y][X]=T;
    for(int i=0;i<4;i++)
    {
        Change(X+Dir[i][0],Y+Dir[i][1],T);
        Change(X+Dir[i][0]*2,Y+2*Dir[i][1],T);
    }
}
void SetBoomTime()
{
    memset(BombMap,0,sizeof(BombMap));
    for(int i=1;i<=BombNum;i++)
    {
        int X=BombX[i];
        int Y=BombY[i];
        BombMap[Y][X]=BombTime[i];
    }
    for(int i=1;i<=BombNum;i++)
    {
        int X=BombX[i];
        int Y=BombY[i];
        Change(Y,X,BombMap[Y][X]);
    }
    for(int i=1;i<=BombNum;i++)
    {
        int X=BombX[i];
        int Y=BombY[i];
        BombTime[i]=BombMap[Y][X];
    }
}
int CoordToNum(int X,int Y)
{
    return Y*MAXLEN+X;
}
void BulidLinkMap()
{
    int TempMap[MAXLEN][MAXLEN];
    GetSourceMap(SourceMap,TempMap);
    for(int i=1;i<=BombNum;i++)
    {
        int X=BombX[i];
        int Y=BombY[i];
        TempMap[Y][X]=BombTime[i];
    }
    for(int y=1;y<=MAPLEN;y++)
        for(int x=1;x<=MAPLEN;x++)
        {
            int At=CoordToNum(x,y);
            int X,Y;
            for(int i=0;i<4;i++)
            {
                X=x+Dir[i][1];
                Y=y+Dir[i][0];
            }
            switch(TempMap[Y][X])
            {
                case WOOD:
                    Link[At].push_back(Edge(CoordToNum(X,Y),9));
                case BLANK:
                    Link[At].push_back(Edge(CoordToNum(X,Y),1));
                case STONE:
                    Link[At].push_back(Edge(CoordToNum(X,Y),INF));
                case HOME:
                    Link[At].push_back(Edge(CoordToNum(X,Y),INF));
                case BOMB:
                    Link[At].push_back(Edge(CoordToNum(X,Y),BombMap[Y][X]));
            }
        }
}
void Dijistra(int Source)
{
    int Visit[MAXROOM];
    memset(Visit,0,sizeof(Visit));
    fill(Short,Short+MAXROOM,INF);
    Short[Source]=0;
    Path[Source]=Source;
    while(true)
    {
        int Pos=-1;
        for(int i=0;i<MAXROOM;i++)
            if(!Visit[i]&&(Pos==-1||Visit[i]<Visit[Pos]))
                Pos=i;
        Visit[Pos]=true;
        //cout<<Point<<endl;
        if(Pos==-1)
            break;
        for(int i=0;i<Link[Pos].size();i++)
        {
            int Dest=Link[Pos][i].Dest;
            int Cost=Link[Pos][i].Cost;
            if(Short[Dest]>Short[Pos]+Cost)
            {
                Short[Dest]=Short[Pos]+Cost;
                Path[Dest]=Pos;
            }            
        }
    }
}
int GetDest(int ID)
{






}
//########################
//------------------------
//########################
// Escape Function
Coord PosOp(Coord Pos,int op)
{
    op=op-1;
    Pos.y+=Dir[op][0];
    Pos.x+=Dir[op][1];
    return Pos;
}
int SafeMap[MAXLEN][MAXLEN];
int Attion[MAXLEN][MAXLEN];
int PathMap[MAXLEN][MAXLEN];
int StepMap[MAXLEN][MAXLEN];
void BulidSafeMap()
{
    GetSourceMap(SourceMap,SafeMap);
    for(int i=0;i<BombNum;i++)
    {
        int X=BombX[i];
        int Y=BombY[i];
        SafeMap[Y][X]=BOMB;
        Attion[Y][X]=ATTENTION;
        for(int i=0;i<4;i++)
            for(int j=1;j<=2;j++)
            {
                int TX=X+Dir[i][1]*j;
                int TY=Y+Dir[i][0]*j;
                if(SafeMap[TY][TX]!=BLANK)
                    break;
                Attion[TY][TX]=ATTENTION;
            }
    }
}
Coord GetPath(Coord Source)
{
    queue<Coord> Next;
    memset(StepMap,-1,sizeof(StepMap));
    memset(StepMap,0,sizeof(PathMap));
    Next.push(Source);
    int X=Source.x;
    int Y=Source.y;
    StepMap[Y][X]=0;
    PathMap[Y][X]=0;
    if(Attion[Y][X]!=ATTENTION)
        return Source;
    while(Next.empty())
    {
        Coord Now;
        Now=Next.front();
        Next.pop();
        X=Now.x;
        Y=Now.y;
        for(int i=0;i<4;i++)
        {
            int TX=X+Dir[i][1];
            int TY=Y+Dir[i][0];
            if(SafeMap[TY][TX]!=BLANK)
                continue;
            if(PathMap[TY][TX]!=-1)
                continue;
            StepMap[TY][TX]=PathMap[Y][X]+1;
            PathMap[TY][TX]=i+1;
            Coord Temp;
            Temp.x=TX;
            Temp.y=TY;
            Next.push(Temp);
            if(Attion[TY][TX]!=ATTENTION)
                return Temp;
        }
    }
    Coord  Ans;
    Ans.x=-1;
    Ans.y=-1;
    return Ans;
}
int GetStep(Coord Pos)
{
    if(Pos.x<0||Pos.y<0)
        return INF;
    return StepMap[Pos.y][Pos.x];
}
Coord OptoPos(Coord Pos,int Op)
{
    if(Op>=1&&Op<=4)
    {
        Pos.x+=Dir[Op-1][1];
        Pos.y+=Dir[Op-1][0];
    }
    return Pos;
}
int GetInvOp(int Op)
{
    switch(Op)
    {
        case UP:
            return DOWN;
        case DOWN:
            return UP;
        case LEFT:
            return RIGHT;
        case RIGHT:
            return LEFT;
    }
    return Op;
}
int GetOp(Coord Pos)
{
    if(Pos.x<0||Pos.y<0)
        return PUT;
    int Ans=NOP;
    while(PathMap[Pos.y][Pos.x]!=NOP)
    {
        Ans=PathMap[Pos.y][Pos.x];
        Pos=OptoPos(Pos,GetInvOp(Ans));
    }
    return Ans;
}
void Judge(int ID)
{
    Coord Pos=MyPlayer[ID].pos;
    int Op=Behave[ID];
    int StepA,StepS;
    BulidSafeMap();
    Coord Temp;
    Temp=GetPath(Pos);
    StepA=GetStep(Temp);
    Pos=OptoPos(Pos,Op);
    Temp=GetPath(Pos);
    StepS=GetStep(Temp);
    if(StepS<StepA)
        Behave[ID]=GetOp(Temp);
}
//########################
//------------------------
//########################
void AI(const Game *game,Operator op[PLAYER_NUM])
{
    GetGame(game);
    if(Round<=TABLE)
        AutoRun(Round);
    else 
    {
        
    }
    Push(op);
}
//########################
bool TrueOp(Coord Pos,int Map)
{

}
