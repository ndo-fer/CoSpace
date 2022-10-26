////////////////////////////////////////
//
//	File : ai.c
//	CoSpace Robot
//	Version 1.0.0
//	OCT 1 2021
//	Copyright (C) 2021 CoSpace Robot. All Rights Reserved
//
//////////////////////////////////////
//
// ONLY C Code can be compiled.
//
/////////////////////////////////////

#define CsBot_AI_H//DO NOT delete this line
#ifndef CSBOT_REAL
#include <windows.h>
#include <stdio.h>
#include <math.h>
#define DLL_EXPORT extern __declspec(dllexport)
#define false 0
#define true 1
#endif
//The robot ID : six chars unique CID.
//Find it from your CoSpace Robot label or CoSpace program download GUI.
//Don't write the below line into two lines or more lines.
char AI_MyID[6] = {'1','2','3','4','5','6'};

int Duration = 0;
int SuperDuration = 0;
int bGameEnd = false;
int CurAction = -1;
int CurGame = 0;
int SuperObj_Num = 0;
int SuperObj_X = 0;
int SuperObj_Y = 0;
int Teleport = 0;
int LoadedObjects = 0;
int Cyan = 0;
int Red = 0;
int Black = 0;
int US_Front = 0;
int US_Left = 0;
int US_Right = 0;
int CSLeft_R = 0;
int CSLeft_G = 0;
int CSLeft_B = 0;
int CSRight_R = 0;
int CSRight_G = 0;
int CSRight_B = 0;
int PositionX = 0;
int PositionY = 0;
int Compass = 0;
int Time = 0;
int WheelLeft = 0;
int WheelRight = 0;
int LED_1 = 0;
int MyState = 0;
int AI_TeamID = 1;   //Robot Team ID.    1:Blue Ream;    2:Red Team.
int AI_SensorNum = 12;

#define CsBot_AI_C//DO NOT delete this line

DLL_EXPORT void SetGameID(int GameID)
{
    if(CurGame != GameID) LoadedObjects = 0;
    CurGame = GameID;
    bGameEnd = 0;
}


DLL_EXPORT void SetTeamID(int TeamID)
{
    AI_TeamID = TeamID;
}

DLL_EXPORT int GetGameID()
{
    return CurGame;
}

//Only Used by CsBot Dance Platform
DLL_EXPORT int IsGameEnd()
{
    return bGameEnd;
}

#ifndef CSBOT_REAL

char info[3000];
DLL_EXPORT char* GetDebugInfo()
{
    sprintf(info, "Duration=%d;SuperDuration=%d;bGameEnd=%d;CurAction=%d;CurGame=%d;SuperObj_Num=%d;SuperObj_X=%d;SuperObj_Y=%d;Teleport=%d;LoadedObjects=%d;Cyan=%d;Red=%d;Black=%d;US_Front=%d;US_Left=%d;US_Right=%d;CSLeft_R=%d;CSLeft_G=%d;CSLeft_B=%d;CSRight_R=%d;CSRight_G=%d;CSRight_B=%d;PositionX=%d;PositionY=%d;Compass=%d;Time=%d;WheelLeft=%d;WheelRight=%d;LED_1=%d;MyState=%d;",Duration,SuperDuration,bGameEnd,CurAction,CurGame,SuperObj_Num,SuperObj_X,SuperObj_Y,Teleport,LoadedObjects,Cyan,Red,Black,US_Front,US_Left,US_Right,CSLeft_R,CSLeft_G,CSLeft_B,CSRight_R,CSRight_G,CSRight_B,PositionX,PositionY,Compass,Time,WheelLeft,WheelRight,LED_1,MyState);
    return info;
}
 
DLL_EXPORT char* GetTeamName()
{
     return "o_o";
}

DLL_EXPORT int GetCurAction()
{
    return CurAction;
}

//Only Used by CsBot Rescue Platform
DLL_EXPORT int GetTeleport()
{
    return Teleport;
}

//Only Used by CsBot Rescue Platform
DLL_EXPORT void SetSuperObj(int X, int Y, int num)
{
    SuperObj_X = X;
    SuperObj_Y = Y;
    SuperObj_Num = num;
}
//Only Used by CsBot Rescue Platform
DLL_EXPORT void GetSuperObj(int *X, int *Y, int *num)
{
    *X = SuperObj_X;
    *Y = SuperObj_Y;
    *num = SuperObj_Num;
}

#endif ////CSBOT_REAL

DLL_EXPORT void SetDataAI(volatile int* packet, volatile int *AI_IN)
{

    int sum = 0;

    US_Front = AI_IN[0]; packet[0] = US_Front; sum += US_Front;
    US_Left = AI_IN[1]; packet[1] = US_Left; sum += US_Left;
    US_Right = AI_IN[2]; packet[2] = US_Right; sum += US_Right;
    CSLeft_R = AI_IN[3]; packet[3] = CSLeft_R; sum += CSLeft_R;
    CSLeft_G = AI_IN[4]; packet[4] = CSLeft_G; sum += CSLeft_G;
    CSLeft_B = AI_IN[5]; packet[5] = CSLeft_B; sum += CSLeft_B;
    CSRight_R = AI_IN[6]; packet[6] = CSRight_R; sum += CSRight_R;
    CSRight_G = AI_IN[7]; packet[7] = CSRight_G; sum += CSRight_G;
    CSRight_B = AI_IN[8]; packet[8] = CSRight_B; sum += CSRight_B;
    PositionX = AI_IN[9]; packet[9] = PositionX; sum += PositionX;
    PositionY = AI_IN[10]; packet[10] = PositionY; sum += PositionY;
    Compass = AI_IN[11]; packet[11] = Compass; sum += Compass;
    Time = AI_IN[12]; packet[12] = Time; sum += Time;
    packet[13] = sum;

}
DLL_EXPORT void GetCommand(int *AI_OUT)
{
    AI_OUT[0] = WheelLeft;
    AI_OUT[1] = WheelRight;
    AI_OUT[2] = LED_1;
}
void TurnTo(int curRot, int targetRot)
{
    int p0 = targetRot;
    int p3 = (targetRot + 3) % 360;
    int p15 = (targetRot + 15) % 360;
    int n3 = (targetRot - 3 + 360) % 360;
    int n15 = (targetRot - 15 + 360) % 360;
    int p180 = (targetRot + 180) % 360;
    int l = 0, r = 0;
    Duration = 6;
    //Within(-3,+3)deg, stop turing.
    l = n3; r = p3;
    if ((l < r && curRot > l && curRot < r) ||
    (l > r && (curRot > l || curRot < r)))
    {
        WheelLeft = 0;
        WheelRight = 0;
        Duration = 0;
        return;
    }
    //Within[3,15]deg,Turn Slowly
    l = p3; r = p15;
    if ((l < r && curRot >= l && curRot <= r) ||
        (l > r && (curRot >= l || curRot <= r)))
    {
        WheelLeft = 10;
        WheelRight = -10;
        return;
    }
    //Within[15,180]deg,Turn Faast
    l = p15; r = p180;
    if ((l < r && curRot >= l && curRot <= r) ||
       (l > r && (curRot >= l || curRot <= r)))
    {
        WheelLeft = 30;
        WheelRight = -30;
        return;
    }
    //Within[-15,-3]deg,Turn Slowly
    l = n15; r = n3;
    if ((l < r && curRot >= l && curRot <= r) ||
    (l > r && (curRot >= l || curRot <= r)))
    {
        WheelLeft = -10;
        WheelRight = 10;
        return;
    }
    //Within[-180,-15]deg,Turn Fast
    l = p180; r = n15;
    if ((l < r && curRot >= l && curRot <= r) ||
    (l > r && (curRot >= l || curRot <= r)))
    {
        WheelLeft = -30;
        WheelRight = 30;
        return;
    }
}
void Game0()
{

    if(Duration>0)
    {
        Duration--;
    }
    else if(CSLeft_R>=200 && CSLeft_G>=80 && CSLeft_G<=140 && CSLeft_B<=40 && CSRight_R>=200 && CSRight_G>=80 && CSRight_G<=140 && CSRight_B<=40&&(LoadedObjects>3))
    {
        Duration = 75;
        CurAction =1;
    }
    else if(CSLeft_R>=80 && CSLeft_G<=80 && CSLeft_B>=200&&(LoadedObjects<7))
    {
        Duration = 65;
        CurAction =2;
    }
    else if(CSRight_R>=80 && CSRight_G<=80 && CSRight_B>=200&&(LoadedObjects<7))
    {
        Duration = 65;
        CurAction =3;
    }
    else if(CSLeft_R>=200 && CSLeft_G<=60 && CSLeft_B<=60&&(LoadedObjects<4))
    {
        Duration = 65;
        CurAction =4;
    }
    else if(CSRight_R>=200 && CSRight_G<=60 && CSRight_B<=60&&(LoadedObjects<4))
    {
        Duration = 65;
        CurAction =5;
    }
    else if(CSLeft_R<=50 && CSLeft_G<=50 && CSLeft_B<=50&&(LoadedObjects<6))
    {
        Duration = 65;
        CurAction =6;
    }
    else if(CSRight_R<=50 && CSRight_G<=50 && CSRight_B<=50&&(LoadedObjects<6))
    {
        Duration = 65;
        CurAction =7;
    }
    else if(CSLeft_R<=35 && CSLeft_G>=210 && CSLeft_B>=210&&(LoadedObjects<7
))
    {
        Duration = 65;
        CurAction =8;
    }
    else if(CSRight_R<=35 && CSRight_G>=210 && CSRight_B>=210&&(LoadedObjects<7
))
    {
        Duration = 65;
        CurAction =9;
    }
    else if(CSLeft_R>=200 && CSLeft_G>=200 && CSLeft_B<=50&&(LoadedObjects>0))
    {
        Duration = 3;
        CurAction =10;
    }
    else if(CSRight_R>=200 && CSRight_G>=200 && CSRight_B<=50&&(LoadedObjects>0))
    {
        Duration = 3;
        CurAction =11;
    }
    else if(US_Front>=15 && US_Left>=10 && US_Right>=10)
    {
        Duration = 0;
        CurAction =12;
    }
    else if(US_Right<=20)
    {
        Duration = 1;
        CurAction =13;
    }
    else if(US_Left<=20)
    {
        Duration = 1;
        CurAction =14;
    }
    else if(US_Front<=15 && US_Right>=20)
    {
        Duration = 3;
        CurAction =15;
    }
    else if(US_Front<=15 && US_Left>=20)
    {
        Duration = 3;
        CurAction =16;
    }
    switch(CurAction)
    {
        case 1:
            WheelLeft=0;
            WheelRight=0;
            LED_1=2;
            if(Duration == 1) {LoadedObjects = 0;} 
            if(Duration < 16)
            {
                WheelLeft = -50;
                WheelRight = -50;
            }
            if(Duration < 8)
            {
                WheelLeft = -50;
                WheelRight = +50;
            }
            break;
        case 2:
            WheelLeft=0;
            WheelRight=0;
            LED_1=1;
            if(Duration == 1) LoadedObjects++;
            if(Duration < 6)
            {
                WheelLeft = 40;
                WheelRight = 40;
            }
            break;
        case 3:
            WheelLeft=0;
            WheelRight=0;
            LED_1=1;
            if(Duration == 1) LoadedObjects++;
            if(Duration < 6)
            {
                WheelLeft = 40;
                WheelRight = 40;
            }
            break;
        case 4:
            WheelLeft=0;
            WheelRight=0;
            LED_1=1;
            if(Duration == 1) LoadedObjects++;
            if(Duration < 6)
            {
                WheelLeft = 40;
                WheelRight = 40;
            }
            break;
        case 5:
            WheelLeft=0;
            WheelRight=0;
            LED_1=1;
            if(Duration == 1) LoadedObjects++;
            if(Duration < 6)
            {
                WheelLeft = 40;
                WheelRight = 40;
            }
            break;
        case 6:
            WheelLeft=0;
            WheelRight=0;
            LED_1=1;
            if(Duration == 1) LoadedObjects++;
            if(Duration < 6)
            {
                WheelLeft = 40;
                WheelRight = 40;
            }
            break;
        case 7:
            WheelLeft=0;
            WheelRight=0;
            LED_1=1;
            if(Duration == 1) LoadedObjects++;
            if(Duration < 6)
            {
                WheelLeft = 40;
                WheelRight = 40;
            }
            break;
        case 8:
            WheelLeft=0;
            WheelRight=0;
            LED_1=1;
            if(Duration == 1) LoadedObjects++;
            if(Duration < 6)
            {
                WheelLeft = 40;
                WheelRight = 40;
            }
            break;
        case 9:
            WheelLeft=0;
            WheelRight=0;
            LED_1=1;
            if(Duration == 1) LoadedObjects++;
            if(Duration < 6)
            {
                WheelLeft = 40;
                WheelRight = 40;
            }
            break;
        case 10:
            WheelLeft=40;
            WheelRight=-30;
            LED_1=0;
            break;
        case 11:
            WheelLeft=-30;
            WheelRight=40;
            LED_1=0;
            break;
        case 12:
            WheelLeft=100;
            WheelRight=100;
            LED_1=0;
            break;
        case 13:
            WheelLeft=-20;
            WheelRight=60;
            LED_1=0;
            break;
        case 14:
            WheelLeft=60;
            WheelRight=-20;
            LED_1=0;
            break;
        case 15:
            WheelLeft=-70;
            WheelRight=-30;
            LED_1=0;
            break;
        case 16:
            WheelLeft=-30;
            WheelRight=-70;
            LED_1=0;
            break;
        default:
            break;
    }

}

void Game1()
{

    if(Duration>0)
    {
        Duration--;
    }
    else if(Time<50)
    {
        Duration = 0;
        CurAction =1;
    }
    else if(CSLeft_R>=200 && CSLeft_G>=80 && CSLeft_G<=140 && CSLeft_B<=40 && CSRight_R>=200 && CSRight_G>=80 && CSRight_G<=140 && CSRight_B<=40&&(LoadedObjects>3||(Time>24000&&LoadedObjects>0)))
    {
        Duration = 75;
        CurAction =2;
    }
    else if(CSLeft_R>=200 && CSLeft_G>=80 && CSLeft_G<=140 && CSLeft_B<=40&&(LoadedObjects>3))
    {
        Duration = 3;
        CurAction =3;
    }
    else if(CSRight_R>=200 && CSRight_G>=80 && CSRight_G<=140 && CSRight_B<=40&&(LoadedObjects>3))
    {
        Duration = 3;
        CurAction =4;
    }
    else if(CSLeft_R>=80 && CSLeft_G<=80 && CSLeft_B>=200&&(LoadedObjects<6))
    {
        Duration = 65;
        CurAction =5;
    }
    else if(CSRight_R>=80 && CSRight_G<=80 && CSRight_B>=200&&(LoadedObjects<6
))
    {
        Duration = 65;
        CurAction =6;
    }
    else if(CSLeft_R<=35 && CSLeft_G>=210 && CSLeft_B>=210&&(LoadedObjects<6&&Cyan<198))
    {
        Duration = 65;
        CurAction =7;
    }
    else if(CSRight_R<=35 && CSRight_G>=210 && CSRight_B>=210&&(LoadedObjects<6&&Cyan<198))
    {
        Duration = 65;
        CurAction =8;
    }
    else if(CSLeft_R<=50 && CSLeft_G<=50 && CSLeft_B<=50&&(LoadedObjects<6&&Black<198))
    {
        Duration = 65;
        CurAction =9;
    }
    else if(CSRight_R<=50 && CSRight_G<=50 && CSRight_B<=50&&(LoadedObjects<6&&Black<198))
    {
        Duration = 65;
        CurAction =10;
    }
    else if(CSLeft_R>=200 && CSLeft_G<=60 && CSLeft_B<=60&&(LoadedObjects<6&&Red<198))
    {
        Duration = 65;
        CurAction =11;
    }
    else if(CSRight_R>=200 && CSRight_G<=60 && CSRight_B<=60&&(LoadedObjects<6&&Red<198))
    {
        Duration = 65;
        CurAction =12;
    }
    else if(CSLeft_R>=200 && CSLeft_G>=200 && CSLeft_B<=50&&(LoadedObjects!=0))
    {
        Duration = 3;
        CurAction =13;
    }
    else if(CSRight_R>=200 && CSRight_G>=200 && CSRight_B<=50&&(LoadedObjects!=0))
    {
        Duration = 3;
        CurAction =14;
    }
    else if(US_Front>=15 && US_Left>=15 && US_Right>=15)
    {
        Duration = 0;
        CurAction =15;
    }
    else if(US_Front<=17 && US_Right<=15)
    {
        Duration = 7;
        CurAction =16;
    }
    else if(US_Front<=17 && US_Left<=15)
    {
        Duration = 7;
        CurAction =17;
    }
    else if(US_Front<=15 && US_Right<=10)
    {
        Duration = 5;
        CurAction =18;
    }
    else if(US_Front<=15 && US_Left<=10)
    {
        Duration = 5;
        CurAction =19;
    }
    else if((US_Left<=5)||(US_Right<=5)||(US_Front<=5))
    {
        Duration = 1;
        CurAction =20;
    }
    else if(PositionX<=0 && PositionY<=0 && Compass<=270&&(LoadedObjects>3))
    {
        Duration = 0;
        CurAction =21;
    }
    else if(PositionX<=0 && PositionY>=1 && PositionY<=1 && Compass>=45 && Compass<=135&&(LoadedObjects>3))
    {
        Duration = 0;
        CurAction =22;
    }
    else if(PositionX>=1 && PositionX<=1 && PositionY>=2 && Compass>=225 && Compass<=305&&(LoadedObjects>3))
    {
        Duration = 0;
        CurAction =23;
    }
    else if(PositionX>=2 && PositionY<=0 && Compass>=90&&(LoadedObjects>3))
    {
        Duration = 0;
        CurAction =24;
    }
    else if(PositionX<=0 && PositionY>=2 && Compass<=180&&(LoadedObjects>3||(Compass>=270&&Compass<=360)))
    {
        Duration = 0;
        CurAction =25;
    }
    else if(PositionX>=2 && PositionY>=2 && Compass>=180&&(LoadedObjects>3||(Compass>=0&&Compass<=90)))
    {
        Duration = 0;
        CurAction =26;
    }
    switch(CurAction)
    {
        case 1:
            WheelLeft=0;
            WheelRight=0;
            LED_1=0;
            TurnTo(Compass,180);
            break;
        case 2:
            WheelLeft=0;
            WheelRight=0;
            LED_1=2;
            if(Duration == 1) {LoadedObjects = 0;} 
            if(Duration < 16)
            {
                WheelLeft = -50;
                WheelRight = -50;
            }
            if(Duration < 8)
            {
                WheelLeft = -50;
                WheelRight = +50;
            }
            Cyan = 0;
                    
Red = 0;
                    
Black = 0;
                    
            break;
        case 3:
            WheelLeft=0;
            WheelRight=40;
            LED_1=0;
            break;
        case 4:
            WheelLeft=40;
            WheelRight=0;
            LED_1=0;
            break;
        case 5:
            WheelLeft=0;
            WheelRight=0;
            LED_1=1;
            if(Duration == 1) LoadedObjects++;
            if(Duration < 6)
            {
                WheelLeft = 40;
                WheelRight = 40;
            }
            break;
        case 6:
            WheelLeft=0;
            WheelRight=0;
            LED_1=1;
            if(Duration == 1) LoadedObjects++;
            if(Duration < 6)
            {
                WheelLeft = 40;
                WheelRight = 40;
            }
            break;
        case 7:
            WheelLeft=0;
            WheelRight=0;
            LED_1=1;
            if(Duration == 1) LoadedObjects++;
            if(Duration < 6)
            {
                WheelLeft = 40;
                WheelRight = 40;
            }
            Cyan++;
                    
            break;
        case 8:
            WheelLeft=0;
            WheelRight=0;
            LED_1=1;
            if(Duration == 1) LoadedObjects++;
            if(Duration < 6)
            {
                WheelLeft = 40;
                WheelRight = 40;
            }
            Cyan++;
                    
            break;
        case 9:
            WheelLeft=0;
            WheelRight=0;
            LED_1=1;
            if(Duration == 1) LoadedObjects++;
            if(Duration < 6)
            {
                WheelLeft = 40;
                WheelRight = 40;
            }
            Black++;
                    
            break;
        case 10:
            WheelLeft=0;
            WheelRight=0;
            LED_1=1;
            if(Duration == 1) LoadedObjects++;
            if(Duration < 6)
            {
                WheelLeft = 40;
                WheelRight = 40;
            }
            Black++;
                    
            break;
        case 11:
            WheelLeft=0;
            WheelRight=0;
            LED_1=1;
            if(Duration == 1) LoadedObjects++;
            if(Duration < 6)
            {
                WheelLeft = 40;
                WheelRight = 40;
            }
            Red++;
                    
            break;
        case 12:
            WheelLeft=0;
            WheelRight=0;
            LED_1=1;
            if(Duration == 1) LoadedObjects++;
            if(Duration < 6)
            {
                WheelLeft = 40;
                WheelRight = 40;
            }
            Red++;
                    
            break;
        case 13:
            WheelLeft=-100;
            WheelRight=-40;
            LED_1=0;
            break;
        case 14:
            WheelLeft=-30;
            WheelRight=-80;
            LED_1=0;
            break;
        case 15:
            WheelLeft=100;
            WheelRight=100;
            LED_1=0;
            break;
        case 16:
            WheelLeft=-40;
            WheelRight=50;
            LED_1=0;
            break;
        case 17:
            WheelLeft=60;
            WheelRight=-30;
            LED_1=0;
            break;
        case 18:
            WheelLeft=-50;
            WheelRight=-70;
            LED_1=0;
            break;
        case 19:
            WheelLeft=-70;
            WheelRight=-50;
            LED_1=0;
            break;
        case 20:
            WheelLeft=-80;
            WheelRight=-60;
            LED_1=0;
            break;
        case 21:
            WheelLeft=0;
            WheelRight=0;
            LED_1=0;
            TurnTo(Compass,270);
            break;
        case 22:
            WheelLeft=0;
            WheelRight=0;
            LED_1=0;
            TurnTo(Compass,270);
            break;
        case 23:
            WheelLeft=0;
            WheelRight=0;
            LED_1=0;
            TurnTo(Compass,90);
            break;
        case 24:
            WheelLeft=0;
            WheelRight=0;
            LED_1=0;
            TurnTo(Compass,90);
            break;
        case 25:
            WheelLeft=0;
            WheelRight=0;
            LED_1=0;
            TurnTo(Compass,180);
            break;
        case 26:
            WheelLeft=0;
            WheelRight=0;
            LED_1=0;
            TurnTo(Compass,180);
            break;
        default:
            break;
    }

}


DLL_EXPORT void OnTimer()
{
    switch (CurGame)
    {
        case 9:
            break;
        case 10:
            WheelLeft=0;
            WheelRight=0;
            LED_1=0;
            break;
        case 0:
            Game0();
            break;
        case 1:
            Game1();
            break;
        default:
            break;
    }
}

