#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <stdint.h>
#include <vector>


using namespace std;

#define map_y 21
#define map_x 79



class bomb{
public:
  int placed;
  int fuse;
  int xbomb;
  int ybomb;
  int power;
  int id;
};


  class character{
  public:
    int health;
    int xpos;
    int ypos;
    int power;
    int bombCount;
    int id;
    std::vector<bomb> bombs;
  };


class map{
public:
  int terrain[map_y][map_x];
  character charArr[4];

};

int turn =0;
std::vector<bomb> placedBombs;
std::vector<bomb> detonatedBombs;



static void placeBomb(character *c, map *m){
  bomb b;
  b.xbomb=c->xpos;
  b.ybomb=c->ypos;
  b.id=c->id;
  b.placed=1;
  b.fuse=5;
  b.power=c->power;
  c->bombCount--;
  placedBombs.push_back(b);
}

static bomb createBomb(){
  bomb b;
  b.placed=0;
  b.xbomb=-100;
  b.ybomb=-100;
  return b;
}

  static void initMap(map *m){
    int i,j;
     for(i=0;i<map_y;i++){
       for(j=0;j<map_x;j++){
	 if(i==0||i==20){
	   m->terrain[i][j]=0;
	 }
	 else if(j==0 || j==78){
	   m->terrain[i][j]=0;
	 }
	 else{
	   if(i%2==0 && j%2==0 ){
	     m->terrain[i][j]=2;
	   }
	   else{
	     m->terrain[i][j]=1;
	   }
	 }
	}
      }
     
     //spawn zones tiles each corner and above and besides where boxes cant spawn
     //topleft
     m->terrain[1][1]=9;
     m->terrain[1][2]=9;
     m->terrain[2][1]=9;
     //bottom right
     m->terrain[19][77]=9;
     m->terrain[18][77]=9;
     m->terrain[19][76]=9;
     //top right
     m->terrain[1][77]=9;
     m->terrain[1][76]=9;
     m->terrain[2][77]=9;
     //bottom left
     m->terrain[19][1]=9;
     m->terrain[18][1]=9;
     m->terrain[19][2]=9;
     
     int boxcount=425;
     int randy,randx;
     while(boxcount!=0){
       randy = rand()%20;
       randx = rand()%78;
       if(randx!=1&&randy!=1 ||randx!=77 && randy!=1 || randx!=77 && randy!=19 || randx!= 1 && randy!=19){
	 if(m->terrain[randy][randx]==1){
	   	 m->terrain[randy][randx]=3;
		 boxcount--;
	 }

       }       
     }
     int powerUps=rand()%21+9;
     while(powerUps!=0){
       randy = rand()%15+5;
       randx = rand()%73+5;
       if(m->terrain[randy][randx]==1){
	 m->terrain[randy][randx]=8;
       }
       powerUps--;
     }
  }


void initScreen(void){
  initscr();
  raw();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
}

static void renderMap(map *m){
  clear();
  int i,j;
   int p;

    for(i=0;i<map_y;i++){
     for(j=0;j<map_x;j++){
        if(m->terrain[i][j]==7 || m->terrain[i][j]==9){
   	 m->terrain[i][j]=1;
	  }
       }
     }

   for(p=0;p<placedBombs.size();p++){
      bomb tmp =placedBombs.at(p);
      m->terrain[tmp.ybomb][tmp.xbomb]=6;
   }

   while(detonatedBombs.size()>0){
      bomb tmp =detonatedBombs.at(0);
      int bombP=tmp.power*2;
      int tmpid = tmp.id;
        int tmpy=tmp.ybomb;
	int tmpx=tmp.xbomb;
	int tmpP = bombP;
	  while(tmpP>0){
		if(m->terrain[tmpy][tmpx]==0 || m->terrain[tmpy][tmpx]==2){
		  break;
		}
		else if(m->terrain[tmpy][tmpx]==6){
		  int o;
		  bomb tb;
		  for(o=0;o<placedBombs.size();o++){
		    tb = placedBombs.at(o);
		    if(tb.xbomb==tmpx && tb.ybomb==tmpy){
		      detonatedBombs.push_back(tb);
		      placedBombs.erase(placedBombs.begin()+o);
		      
		    }
		  }
		  if(tmpid==1){
		  m->terrain[tmpy][tmpx]=7;
		  }
		  else{
		    m->terrain[tmpy][tmpx]=9;
		  }
		  }
		else{
		  
		  if(tmpid==1){
		  m->terrain[tmpy][tmpx]=7;
		  }
		  else{
		    m->terrain[tmpy][tmpx]=9;
		  }
	  	}
	  	tmpy--;
	  	tmpP--;
	    }
     
	    tmpy=tmp.ybomb;
	    tmpx=tmp.xbomb;
	   tmpP = bombP;
	  while(tmpP>0){
		if(m->terrain[tmpy][tmpx]==0 || m->terrain[tmpy][tmpx]==2){
		  break;
		}
			else if(m->terrain[tmpy][tmpx]==6){
		  int o;
		  bomb tb;
		  for(o=0;o<placedBombs.size();o++){
		    tb = placedBombs.at(o);
		    if(tb.xbomb==tmpx && tb.ybomb==tmpy){
		      detonatedBombs.push_back(tb);
		      placedBombs.erase(placedBombs.begin()+o);
		      
		    }
		  }
		   if(tmpid==1){
		  m->terrain[tmpy][tmpx]=7;
		  }
		  else{
		    m->terrain[tmpy][tmpx]=9;
		  }
		  }
		else{
		   if(tmpid==1){
		  m->terrain[tmpy][tmpx]=7;
		  }
		  else{
		    m->terrain[tmpy][tmpx]=9;
		  }
	  	}
	  	tmpy++;
	  	tmpP--;
	    }

	    tmpy=tmp.ybomb;
	 tmpx=tmp.xbomb;
	tmpP = bombP;
	  while(tmpP>0){
		if(m->terrain[tmpy][tmpx]==0 || m->terrain[tmpy][tmpx]==2 ){
		  break;
		}
			else if(m->terrain[tmpy][tmpx]==6){
		  int o;
		  bomb tb;
		  for(o=0;o<placedBombs.size();o++){
		    tb = placedBombs.at(o);
		    if(tb.xbomb==tmpx && tb.ybomb==tmpy){
		      detonatedBombs.push_back(tb);
		      placedBombs.erase(placedBombs.begin()+o);
		      
		    }
		  }
		   if(tmpid==1){
		  m->terrain[tmpy][tmpx]=7;
		  }
		  else{
		    m->terrain[tmpy][tmpx]=9;
		  }
		  }
		else{
		   if(tmpid==1){
		  m->terrain[tmpy][tmpx]=7;
		  }
		  else{
		    m->terrain[tmpy][tmpx]=9;
		  }
	  	}
	  	tmpx--;
	  	tmpP--;
	    }

	    tmpy=tmp.ybomb;
	    tmpx=tmp.xbomb;
	    tmpP = bombP;
	  while(tmpP>0){
		if(m->terrain[tmpy][tmpx]==0 || m->terrain[tmpy][tmpx]==2){
		  break;
		}	
		else if(m->terrain[tmpy][tmpx]==6){
		  int o;
		  bomb tb;
		  for(o=0;o<placedBombs.size();o++){
		    tb = placedBombs.at(o);
		    if(tb.xbomb==tmpx && tb.ybomb==tmpy){
		      detonatedBombs.push_back(tb);
		      placedBombs.erase(placedBombs.begin()+o);
		      
		    }
		  }
		   if(tmpid==1){
		  m->terrain[tmpy][tmpx]=7;
		  }
		  else{
		    m->terrain[tmpy][tmpx]=9;
		  }
		  }
		else{
		   if(tmpid==1){
		  m->terrain[tmpy][tmpx]=7;
		  }
		  else{
		    m->terrain[tmpy][tmpx]=9;
		  }
	  	}
	  	tmpx++;
	  	tmpP--;
	    }


	  
	  detonatedBombs.erase(detonatedBombs.begin());
      }

   for(i=0;i<map_y;i++){
     for(j=0;j<map_x;j++){
       switch(m->terrain[i][j]){
       case 1:
	 mvaddch(i,j,' ');
	  break;
       case 0:
	 mvaddch(i,j,'+');
	 break;
       case 2:
	 mvaddch(i,j,'#');
	 break;
       case 3:
	 mvaddch(i,j, '*');
	 break;
       case 4:
	 mvaddch(i,j,'P');
	 break;
       case 5:
	 mvaddch(i,j,'N');
	 break;
       case 6:
	 mvaddch(i,j,'@');
	 break;
       case 7:
	 mvaddch(i,j,'&');
	 break;
       case 8:
	 mvaddch(i,j,'^');
	 break;
       case 9:
	 mvaddch(i,j,'&');
	 break;
       }  
     }
   }
   refresh();
}


static void spawnPlayers(map *m){
  int randS;
  randS = rand()%4;
  character pc;
  character npc1;
  character npc2;
  character npc3;
  
npc1.health=npc2.health=npc3.health=pc.health =1;
npc1.power=npc2.power=npc3.power=pc.power =1;
 pc.id=1;
 npc1.id=npc2.id=npc3.id=2;
 pc.bombCount=npc1.bombCount=npc2.bombCount=npc3.bombCount=1;

  if(randS==0){
    pc.xpos =1;
    pc.ypos=1;
    npc1.xpos=77;
    npc1.ypos=19;
    npc2.xpos=77;
    npc2.ypos=1;
    npc3.xpos=1;
    npc3.ypos=19;
  }
  else if(randS==1){
    pc.xpos=77;
    pc.ypos=19;
    npc1.xpos=1;
    npc1.ypos=1;
    npc2.xpos=77;
    npc2.ypos=1;
    npc3.xpos=1;
    npc3.ypos=19;
  }
  else if(randS=2){
     pc.xpos=77;
    pc.ypos=1;
    npc1.xpos=1;
    npc1.ypos=1;
    npc2.xpos=77;
    npc2.ypos=19;
    npc3.xpos=1;
    npc3.ypos=19;
  }
  else{
     pc.xpos=1;
     pc.ypos=19;
     npc1.xpos=1;
     npc1.ypos=1;
     npc2.xpos=77;
     npc2.ypos=19;
     npc3.xpos=77;
     npc3.ypos=1;
  }
  m->terrain[pc.ypos][pc.xpos]=4;
  m->terrain[npc1.ypos][npc1.xpos]=5;
  m->terrain[npc2.ypos][npc2.xpos]=5;
  m->terrain[npc3.ypos][npc3.xpos]=5;
  m->charArr[0]=pc;
  m->charArr[1]=npc1;
  m->charArr[2]=npc2;
  m->charArr[3]=npc3;

}

static int move(map *m){
  int key;
  int check=0;
  switch(key=getch()){
  case 'w':
    m->charArr[0].ypos-=1;
    if(m->terrain[m->charArr[0].ypos][m->charArr[0].xpos]!=1 && m->terrain[m->charArr[0].ypos][m->charArr[0].xpos]!=9 && m->terrain[m->charArr[0].ypos][m->charArr[0].xpos]!=8){
      m->charArr[0].ypos+=1;
    }
    else{
      m->terrain[m->charArr[0].ypos+1][m->charArr[0].xpos]=1;
      if(m->terrain[m->charArr[0].ypos][m->charArr[0].xpos]==8){
	m->charArr[0].power++;
      }
      m->terrain[m->charArr[0].ypos][m->charArr[0].xpos]=4;
    }
    check=1;
    break;
  case 's':
    m->charArr[0].ypos+=1;
    if(m->terrain[m->charArr[0].ypos][m->charArr[0].xpos]!=1 && m->terrain[m->charArr[0].ypos][m->charArr[0].xpos]!=9&& m->terrain[m->charArr[0].ypos][m->charArr[0].xpos]!=8){
      m->charArr[0].ypos-=1;
    }
    else{
      m->terrain[m->charArr[0].ypos-1][m->charArr[0].xpos]=1;
      if(m->terrain[m->charArr[0].ypos][m->charArr[0].xpos]==8){
	m->charArr[0].power++;
      }
      m->terrain[m->charArr[0].ypos][m->charArr[0].xpos]=4;
    }
    check=1;
    break;
  case 'a':
    m->charArr[0].xpos-=1;
    if(m->terrain[m->charArr[0].ypos][m->charArr[0].xpos]!=1 && m->terrain[m->charArr[0].ypos][m->charArr[0].xpos]!=9&& m->terrain[m->charArr[0].ypos][m->charArr[0].xpos]!=8){
      m->charArr[0].xpos+=1;
    }
    else{
      m->terrain[m->charArr[0].ypos][m->charArr[0].xpos+1]=1;
       if(m->terrain[m->charArr[0].ypos][m->charArr[0].xpos]==8){
	m->charArr[0].power++;
       }
      m->terrain[m->charArr[0].ypos][m->charArr[0].xpos]=4;
    }
    check=1;
     break;
  case 'd':
    m->charArr[0].xpos+=1;
    if(m->terrain[m->charArr[0].ypos][m->charArr[0].xpos]!=1 && m->terrain[m->charArr[0].ypos][m->charArr[0].xpos]!=9&& m->terrain[m->charArr[0].ypos][m->charArr[0].xpos]!=8){
      m->charArr[0].xpos-=1;
    }
    else{
      m->terrain[m->charArr[0].ypos][m->charArr[0].xpos-1]=1;
       if(m->terrain[m->charArr[0].ypos][m->charArr[0].xpos]==8){
	m->charArr[0].power++;
      }
      m->terrain[m->charArr[0].ypos][m->charArr[0].xpos]=4;
    }
    check=1;
   break; 
  case 'r':
    check=1;
    break;
  case 'e':
    if(m->charArr[0].bombCount>0 && m->terrain[m->charArr[0].ypos][m->charArr[0].xpos]==4){
      placeBomb(&(m->charArr[0]), m);
    }
    turn--;
    break;
  }
  
  int l;
  if(check==1){
    int detCount=0;



    for(l=0;l<placedBombs.size();l++){
      placedBombs.at(l).fuse--;
    
      if(placedBombs.at(l).fuse==0){
      	detonatedBombs.push_back(placedBombs.at(l));
      	placedBombs.erase(placedBombs.begin());
		l--;
       }
    }
 
  
   
   }
  return key;
  
}

static void npcMove(int q, map *m){
  //npc will have a 40%chance to move towards the pc 40% chance random direction 20% chance random
  //NPCs will detect bombs up to 5 units away they will attempt to move away from it unless they can move to the side then they will have a 70% chance to move to the side
  //NPCs will have a 20% chance of placing a bomb unless they run into a breakable wall then they will place a bomb
  int moveDir = rand()%11+1;
  int bombCheck=0;
  int t;
  int tmpy,tmpx;
  
  //check above
  tmpy= m->charArr[q].ypos;
  tmpx =m->charArr[q].xpos;
  for(t=m->charArr[q].ypos;t<6;t++){
    if(m->terrain[tmpy][tmpx]==6){
      bombCheck=1;
    }
    if(m->terrain[tmpy][tmpx]==0 || m->terrain[tmpy][tmpx]==2){
      break;
    }
    tmpy--;
  }

  //check below
 if(bombCheck==0){
    tmpy= m->charArr[q].ypos;
    tmpx =m->charArr[q].xpos;
    for(t=m->charArr[q].ypos;t<6;t++){
      if(m->terrain[tmpy][tmpx]==6){
	bombCheck=2;
      }
      if(m->terrain[tmpy][tmpx]==0 || m->terrain[tmpy][tmpx]==2){
	break;
      }
      tmpy++;
    }
  }

 //checkleft
  if(bombCheck==0){
     tmpy= m->charArr[q].ypos;
     tmpx =m->charArr[q].xpos;
     for(t=m->charArr[q].xpos;t<6;t++){
       if(m->terrain[tmpy][tmpx]==6){
	 bombCheck=3;
       }
       if(m->terrain[tmpy][tmpx]==0 || m->terrain[tmpy][tmpx]==2){
	 break;
       }
       tmpx--;
     }
  }

  //checkright
 if(bombCheck==0){
   tmpy= m->charArr[q].ypos;
   tmpx =m->charArr[q].xpos;
   for(t=m->charArr[q].xpos;t<6;t++){
     if(m->terrain[tmpy][tmpx]==6){
       bombCheck=4;
     }
     if(m->terrain[tmpy][tmpx]==0 || m->terrain[tmpy][tmpx]==2){
       break;
     }
     tmpx++;
  }
 }

 m->terrain[m->charArr[q].ypos][m->charArr[q].xpos]=1;
 //if a bomb is identified then the NPC's movement will be changed to react to the bomb
 if(bombCheck>0){
   if(bombCheck==1){
     //move down unless blocked
     if(moveDir<=3 && m->terrain[m->charArr[q].ypos+1][m->charArr[q].xpos]!=2 && m->terrain[m->charArr[q].ypos+1][m->charArr[q].xpos]!=0 && m->terrain[m->charArr[q].ypos+1][m->charArr[q].xpos]!=3){
       m->charArr[q].ypos++;
     }
     else if(moveDir<=3 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos+1]!=2 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos+1]!=0 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos+1]!=3){
       m->charArr[q].xpos++;
     }
     else if(moveDir<=3 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos-1]!=2 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos-1]!=0 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos-1]!=3){
       m->charArr[q].xpos--;
     }
     else if(moveDir>3 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos+1]!=2 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos+1]!=0 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos+1]!=3){
       m->charArr[q].xpos++;
     }
     else if(moveDir>3 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos-1]!=2 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos-1]!=0 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos-1]!=3){
       m->charArr[q].xpos--;
     }
     else if(moveDir>3 && m->terrain[m->charArr[q].ypos+1][m->charArr[q].xpos]!=2 && m->terrain[m->charArr[q].ypos+1][m->charArr[q].xpos]!=0 && m->terrain[m->charArr[q].ypos+1][m->charArr[q].xpos]!=3){
         m->charArr[q].ypos++;
     }
     else{
       
     }
   }
   
   if(bombCheck==2){
      //move up unless blocked
     if(moveDir<=3 && m->terrain[m->charArr[q].ypos-1][m->charArr[q].xpos]!=2 && m->terrain[m->charArr[q].ypos-1][m->charArr[q].xpos]!=0 && m->terrain[m->charArr[q].ypos-1][m->charArr[q].xpos]!=3){
       m->charArr[q].ypos--;
     }
     else if(moveDir<=3 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos+1]!=2 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos+1]!=0 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos+1]!=3){
       m->charArr[q].xpos++;
     }
     else if(moveDir<=3 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos-1]!=2 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos-1]!=0 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos-1]!=3){
       m->charArr[q].xpos--;
     }
     else if(moveDir>3 &&m->terrain[m->charArr[q].ypos][m->charArr[q].xpos+1]!=2 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos+1]!=0 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos+1]!=3){
       m->charArr[q].xpos++;
     }
     else if(moveDir>3 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos-1]!=2 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos-1]!=0 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos-1]!=3){
       m->charArr[q].xpos--;
     }
     else if(moveDir>3 && m->terrain[m->charArr[q].ypos-1][m->charArr[q].xpos]!=2 && m->terrain[m->charArr[q].ypos-1][m->charArr[q].xpos]!=0 && m->terrain[m->charArr[q].ypos-1][m->charArr[q].xpos]!=3){
         m->charArr[q].ypos--;
     }
     else{
       
     }
   }
 
   if(bombCheck==3){
      //move right unless blocked
     if(moveDir<=3 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos+1]!=2 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos+1]!=0 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos+1]!=3){
       m->charArr[q].xpos++;
     }
     else if(moveDir<=3 && m->terrain[m->charArr[q].ypos+1][m->charArr[q].xpos]!=2 && m->terrain[m->charArr[q].ypos+1][m->charArr[q].xpos]!=0 && m->terrain[m->charArr[q].ypos+1][m->charArr[q].xpos]!=3){
       m->charArr[q].ypos++;
     }
     else if(moveDir<=3 && m->terrain[m->charArr[q].ypos-1][m->charArr[q].xpos]!=2 && m->terrain[m->charArr[q].ypos-1][m->charArr[q].xpos]!=0 && m->terrain[m->charArr[q].ypos-1][m->charArr[q].xpos]!=3){
       m->charArr[q].ypos--;
     }
     else if(moveDir>3 && m->terrain[m->charArr[q].ypos+1][m->charArr[q].xpos]!=2 && m->terrain[m->charArr[q].ypos+1][m->charArr[q].xpos]!=0 && m->terrain[m->charArr[q].ypos+1][m->charArr[q].xpos]!=3){
       m->charArr[q].ypos++;
     }
     else if(moveDir>3 && m->terrain[m->charArr[q].ypos-1][m->charArr[q].xpos]!=2 && m->terrain[m->charArr[q].ypos-1][m->charArr[q].xpos]!=0 && m->terrain[m->charArr[q].ypos-1][m->charArr[q].xpos]!=3){
       m->charArr[q].ypos--;
     }
     else if(moveDir>3 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos+1]!=2 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos+1]!=0 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos+1]!=3){
         m->charArr[q].xpos++;
     }
     else{
       
     }
   }
   
   if(bombCheck==4){
    //move left unless blocked
     if(moveDir<=3 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos-1]!=2 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos-1]!=0 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos-1]!=3){
       m->charArr[q].xpos--;
     }
     else if(moveDir<=3 && m->terrain[m->charArr[q].ypos+1][m->charArr[q].xpos]!=2 && m->terrain[m->charArr[q].ypos+1][m->charArr[q].xpos]!=0 && m->terrain[m->charArr[q].ypos+1][m->charArr[q].xpos]!=3){
       m->charArr[q].ypos++;
     }
     else if(moveDir<=3 && m->terrain[m->charArr[q].ypos-1][m->charArr[q].xpos]!=2 && m->terrain[m->charArr[q].ypos-1][m->charArr[q].xpos]!=0 && m->terrain[m->charArr[q].ypos-1][m->charArr[q].xpos]!=3){
       m->charArr[q].ypos--;
     }
     else if(moveDir>3 && m->terrain[m->charArr[q].ypos+1][m->charArr[q].xpos]!=2 && m->terrain[m->charArr[q].ypos+1][m->charArr[q].xpos]!=0 && m->terrain[m->charArr[q].ypos+1][m->charArr[q].xpos]!=3){
       m->charArr[q].ypos++;
     }
     else if(moveDir>3 && m->terrain[m->charArr[q].ypos-1][m->charArr[q].xpos]!=2 && m->terrain[m->charArr[q].ypos-1][m->charArr[q].xpos]!=0 && m->terrain[m->charArr[q].ypos-1][m->charArr[q].xpos]!=3){
       m->charArr[q].ypos--;
     }
     else if(moveDir>3 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos+1]!=2 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos+1]!=0 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos+1]!=3){
         m->charArr[q].xpos++;
     }
     else{
       
     }
       
       
   }

 }
 else{
   //no bomb detected
   //npc will have a 40%chance to move towards the pc 60% chance to move into a random position
   //move towards the pc
   if(moveDir <=4){
     if(m->charArr[0].xpos>m->charArr[q].xpos && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos+1]!=2 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos+1]!=0 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos+1]!=3){
       m->charArr[q].xpos++;
     }
     else if(m->charArr[0].xpos<m->charArr[q].xpos && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos-1]!=2 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos-1]!=0 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos-1]!=3){
       m->charArr[q].xpos--;
     }
     else if(m->charArr[0].xpos==m->charArr[q].xpos){
       if(m->charArr[0].ypos>m->charArr[q].ypos && m->terrain[m->charArr[q].ypos+1][m->charArr[q].xpos]!=2 && m->terrain[m->charArr[q].ypos+1][m->charArr[q].xpos]!=0 && m->terrain[m->charArr[q].ypos+1][m->charArr[q].xpos]!=3){
	 m->charArr[q].ypos++;
       }
       else{
	 if(m->terrain[m->charArr[q].ypos-1][m->charArr[q].xpos]!=2 && m->terrain[m->charArr[q].ypos-1][m->charArr[q].xpos]!=0 && m->terrain[m->charArr[q].ypos-1][m->charArr[q].xpos]!=3){
	   m->charArr[q].ypos--;
	 }
       }
     }
   }
   else if(moveDir>4){
     //move random
     if(m->terrain[m->charArr[q].ypos][m->charArr[q].xpos+1]!=2 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos+1]!=0 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos+1]!=3){
     m->charArr[q].xpos++;
     }
     else if(m->terrain[m->charArr[q].ypos][m->charArr[q].xpos-1]!=2 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos-1]!=0 && m->terrain[m->charArr[q].ypos][m->charArr[q].xpos-1]!=3){
     m->charArr[q].xpos--;
     }
     else if(m->terrain[m->charArr[q].ypos+1][m->charArr[q].xpos]!=2 && m->terrain[m->charArr[q].ypos+1][m->charArr[q].xpos]!=0 && m->terrain[m->charArr[q].ypos+1][m->charArr[q].xpos]!=3){
     m->charArr[q].ypos++;
     }
     else if(m->terrain[m->charArr[q].ypos-1][m->charArr[q].xpos]!=2 && m->terrain[m->charArr[q].ypos-1][m->charArr[q].xpos]!=0 && m->terrain[m->charArr[q].ypos-1][m->charArr[q].xpos]!=3){
     m->charArr[q].ypos--;
     }
   }
  
 }
 //place bombs if possible
 if(m->charArr[q].bombCount>0){
   int bombChance = rand()%150+1;
   if(bombChance<=3){
     placeBomb(&(m->charArr[q]),m);
   }
   else if(m->terrain[m->charArr[q].ypos-1][m->charArr[q].xpos]==3){
   placeBomb(&(m->charArr[q]),m);
   }
  else if(m->terrain[m->charArr[q].ypos+1][m->charArr[q].xpos]==3){
   placeBomb(&(m->charArr[q]),m);
   }
  else if(m->terrain[m->charArr[q].ypos][m->charArr[q].xpos+1]==3){
   placeBomb(&(m->charArr[q]),m);
   }
  else if(m->terrain[m->charArr[q].ypos][m->charArr[q].xpos-1]==3){
   placeBomb(&(m->charArr[q]),m);
   }
   }
m->terrain[m->charArr[q].ypos][m->charArr[q].xpos]=5;

}

int main(int argc, char *argv[])
{
   map m;
   time_t seed;
   struct timeval tv;



   gettimeofday(&tv, NULL);
    seed = (tv.tv_usec ^ (tv.tv_sec << 20)) & 0xffffffff;
 srand(seed);

 

  initScreen();
  initMap(&m);
  spawnPlayers(&m);
  renderMap(&m);

  int deathStatus=0;
  int deathcount=0;
  int k;
    while(m.charArr[0].health==1&&k!='q'){
      turn++;
       if(turn%4==0){
      	m.charArr[0].bombCount++;
      	m.charArr[1].bombCount++;
      	m.charArr[2].bombCount++;
	m.charArr[3].bombCount++;
      }
       
       if(m.terrain[m.charArr[0].ypos][m.charArr[0].xpos]==7 || m.terrain[m.charArr[0].ypos][m.charArr[0].xpos]==9){
	 m.charArr[0].health=0;
	 deathStatus=1;
       }
       else{
	  k= move(&m);
       }
       int q;
       for(q=1;q<4;q++){
	 if(m.terrain[m.charArr[q].ypos][m.charArr[q].xpos]==7){
	   m.charArr[q].health=0;
	 }
       }
       for(q=1;q<4;q++){
	 if(m.charArr[q].health==1){
	   //move
	   npcMove(q,&m);
	 }
	 else{
	   deathcount++;
	 }
       }


       if(deathcount==3){
	 deathStatus=2;
	 break;
       }
       else{
	 deathcount=0;
       }
       renderMap(&m);
    }
   
    endwin();
    if(deathStatus==1){
      cout<< "You Lose"<<endl;
    }
    if(deathStatus==2){
      cout<< "You Win"<<endl;
    }
  return 0;
}
