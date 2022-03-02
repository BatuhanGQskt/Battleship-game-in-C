#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define rows 10
#define columns 10
#define number_of_Piece 5

/**
Destroyer 1 piece 2 long type basic
Submarine 2 Piece 3 long type basic
Cruiser 1 piece 3 long type basic
BattleShip 1 piece 4 long type basic
Carrier 1 piece 5 long type Skye Attack (should not be damaged until use, can be used at 3rd round)
*/
enum ShipType { Carrier = 1, Battleship = 2, Cruiser = 3, Submarine = 4, Destroyer = 5 };
enum specialSkills { skyeAttack = 1, deepAttack = 2, none = 0 };

struct ShipStruct {
    int number_of_holes;
    int type;
    int specialSkill;
};

/**
    PlayerPieces holds s_piece as ShipStruct which holds the ship information
    num_of_single_piece used for the count number of pieces player placed
    max_num_of_piece is the number of pieces a player can have of that type
*/
struct PlayerPieces {
    struct ShipStruct* s_piece;
    int num_of_single_piece;
    int max_num_of_piece;
};

struct Point {
    int x;
    int y;
};

struct Player {
    struct PlayerPieces* pieces;
    int num_of_Player;
    int score;
    int** GameBoard;
    int** AttackBoard;
};

int** createBoard(void);
void printBoard(int **);
void tester(int**);
void end_of_Game(struct Player*);
struct Player* setPlayer(int *);

bool isClashed(int**, struct Point, struct Point);

void displayPlayer(struct Player*);
void displaySpecialSkill(int);

struct ShipStruct* setPiece(int);
struct PlayerPieces* setPlayerPieces(struct PlayerPieces*);

void placePiece(struct Player*);
int upSide_or_downSide(int);

int** fillRangeOfBoard(int**, struct Point, struct Point, int);

char* mapShip(int);
char* mapShipSpecialSkill(int);

void displayPieceInfo(struct ShipStruct* );
void displayPlayerPieces(struct PlayerPieces* pp);

void remainedPieces(struct Player* );

//Be aware that number of Pieces set as define value with 5!
int main() {
    int i, num_of_Players = 1;


    struct Player* player1 = setPlayer(&num_of_Players);
    struct Player* player2 = setPlayer(&num_of_Players);

    displayPlayer(player1);
    displayPlayer(player2);

    printBoard(player1->GameBoard);

    tester(player1->GameBoard);

    displayPlayerPieces(player1->pieces);

    for(int i = 0 ;i < number_of_Piece;i++) {
        printBoard(player1->GameBoard);
        placePiece(player1);
    }



    displayPlayer(player1);

    end_of_Game(player1);
    end_of_Game(player2);

    return 0;
}


void gameStart(struct Player* p1) {

}

void displayPieceInfo(struct ShipStruct* s) {
    printf("Number of holes: %d\nSpecial Skill: %s\nShip Type: %s\n",s->number_of_holes,mapShipSpecialSkill(s->specialSkill),mapShip(s->type));
}


void displayPlayerPieces(struct PlayerPieces* pp) {
    int i;

    for(i=0;i < number_of_Piece;i++) {
        printf("Number of placed pieces: %d\n",pp[i].num_of_single_piece);
        printf("Number of maximum pieces: %d\n",pp[i].max_num_of_piece);
        displayPieceInfo(pp[i].s_piece);

        printf("\n\n");
    }
}

/**
ship_size is equal to ship size range(2,6) 6 is not included.
returns 1 if left to right
return 2 if top to bottom
*/
int upSide_or_downSide(int ship_size) {
    int i;

    printf("Size of the ship is %d.\nShip appearance left to right:\n",ship_size);
    for(i=0;i<ship_size;i++) {
        printf("* ");
    }
    printf("\nShip appearance top to bottom:\n");
    for(i=0;i<ship_size;i++) {
        printf("*\n");
    }

    int selection;
    do {
        printf("\nEnter the selection for position (1 for left to right/2 for top to bottom):");
        if( scanf("%d",&selection) == 0 ) {
            fflush(stdin);
        }
    }while((selection!=1 && selection!=2));

    return selection;
}

int** fillRangeOfBoard(int** board, struct Point start, struct Point end, int ship) {
    int i, j;

    for(i = start.x; i < end.x ;i++) {
        for(j = start.y; j < end.y;j++) {
            board[i][j] = ship;
        }
    }
    return board;
}

bool isClashed(int** board, struct Point start, struct Point end) {
    int i, j;

    for(i = start.x; i < end.x ;i++) {
        for(j = start.y; j < end.y;j++) {
            if(board[i][j] != 0) {
                return true;
            }
        }
    }
    return false;
}

//Carrier = 1, Battleship = 2, Cruiser = 3, Submarine = 4, Destroyer = 5
char* mapShip(int shipNum){
    char* name_ship = (char*) malloc(sizeof(char) * 20);
    if(shipNum == 1) {
        strcpy(name_ship,"Carrier");
        name_ship[7] = '\0';

    } else if(shipNum == 2) {
        strcpy(name_ship,"Battleship");
        name_ship[10] = '\0';

    } else if(shipNum == 3) {
        strcpy(name_ship,"Cruiser");
        name_ship[7] = '\0';

    } else if(shipNum == 4) {
        strcpy(name_ship,"Submarine");
        name_ship[9] = '\0';

    } else if(shipNum == 5) {
        strcpy(name_ship,"Destroyer");
        name_ship[9] = '\0';
    }
    return name_ship;
}

//skyeAttack = 1, deepAttack = 2, none = 0
char* mapShipSpecialSkill(int shipSkillNum) {
    char* name_ship_skill = (char*) malloc(sizeof(char) * 20);

    if(shipSkillNum == 1) {
        strcpy(name_ship_skill,"skyeAttack");
        name_ship_skill[10] = '\0';

    } else if(shipSkillNum == 2) {
        strcpy(name_ship_skill,"deepAttack");
        name_ship_skill[10] = '\0';

    } else if(shipSkillNum == 0) {
        strcpy(name_ship_skill,"None");
        name_ship_skill[4] = '\0';
    }
    return name_ship_skill;
}

void placePiece(struct Player* p) {
    char y;
    int x,flagX=1,flagY=1,choosen_ship, ship_placement, flag_loop;

    do {
        flag_loop = 0;
        printf("\nWhich peace will you place?\n");
        printf("1)Carrier\n2)Battleship\n3)Cruiser\n4)Submarine\n5)Destroyer\n");
        printf("Your select: ");
        scanf("%d",&choosen_ship);

        if(choosen_ship<1 || choosen_ship>5){
            printf("Wrong ship number please select between 1 and %d.\n",number_of_Piece);
            flag_loop = 1;
        }

        if (p->pieces[choosen_ship-1].num_of_single_piece >= p->pieces[choosen_ship-1].max_num_of_piece) {
            printf("Please choose another ship than the %s\n", mapShip(choosen_ship));

            flag_loop = 1;
        }

    }while(flag_loop == 1);

    ship_placement = upSide_or_downSide(p->pieces[choosen_ship-1].s_piece->number_of_holes);

    printBoard(p->GameBoard);

    struct Point ship_position_start, ship_position_end;
    do {
        fflush(stdin);
        printf("Enter coordinates (A, 1): ");
        scanf("%c %d",&x,&y);

        if(x<'A' || x>'J') {
            printf("Wrong coordinate entered for x!\n");
            flagX = 0;
        }
        else {
            flagX = 1;
        }
        if(y<1 || y>10) {
            printf("Wrong coordinate entered for y!\n");
            flagY=0;
        } else {
            flagY = 1;
        }

        if(flagX == 1 && flagY == 1) {
            y -= 1;
            int number_of_ship_holes = p->pieces[choosen_ship-1].s_piece->number_of_holes;

            if(ship_placement == 2) {
                ship_position_start.x = (x-65); //65 subtracted because of ascii values
                ship_position_end.x = (x-65) + number_of_ship_holes;

                ship_position_start.y = y;
                ship_position_end.y = y+1;
            }

            else if(ship_placement == 1) {
                ship_position_start.x = (x-65); //65 subtracted because of ascii values
                ship_position_end.x = (x-65) + 1;

                ship_position_start.y = y;
                ship_position_end.y = y + number_of_ship_holes;
            }

            if(ship_position_end.x >= columns || ship_position_end.y >= columns) {
                //Boundary failed!

                printf("Ship placement failed! Choose appropriate location!\n");

                if (ship_placement == 1) {
                    printf("You can place your ship between ('A', 1) and ('J', %d)\n", columns - number_of_ship_holes);
                }
                else if (ship_placement == 2) {
                    printf("You can place your ship between ('A', 1) and ('%c', 10)\n",(columns - number_of_ship_holes + 65), columns);
                }

                flagX = 0;
                flagY = 0;

            } else if (isClashed(p->GameBoard,ship_position_start, ship_position_end)){
                printf("There is a clash with the other ships!\n");

                remainedPieces(p);
                flagX = 0;
                flagY = 0;
            }else {
                p->GameBoard = fillRangeOfBoard(p->GameBoard,ship_position_start, ship_position_end, choosen_ship);
                p->pieces[choosen_ship - 1].num_of_single_piece += 1;
            }
        }
    }while(flagX == 0 || flagY == 0);
}

void displaySpecialSkill(int skill_int) {
    if(skill_int == 1) {
        printf("Skye Attack!\n");
    }
    else if(skill_int == 2) {
        printf("Deep Attack!\n");
    }
    else if(skill_int == 0) {
        printf("None.\n");
    }
}

struct ShipStruct* setPiece(int t_type) {
    struct ShipStruct* ship = (struct ShipStruct*)malloc(sizeof(struct ShipStruct));

    if(t_type == 1) {
        ship->type = Carrier;
        ship->number_of_holes = 5;
        ship->specialSkill = skyeAttack;
    }
    else if(t_type == 2) {
        ship->type = Battleship;
        ship->number_of_holes = 4;
        ship->specialSkill = none;
    }
    else if(t_type == 3) {
        ship->type = Cruiser;
        ship->number_of_holes = 3;
        ship->specialSkill = none;
    }
    else if(t_type == 4) {
        ship->type = Submarine;
        ship->number_of_holes = 3;
        ship->specialSkill = none;
    }
    else if(t_type == 5) {
        ship->type = Destroyer;
        ship->number_of_holes = 2;
        ship->specialSkill = none;
    }
    else {
        printf("Set piece cannot reach here! with value of t_type: %d\n",t_type);
        return -42;
    }
    return ship;
}

//indexes: 0        1           2       3           4
//meaning: Carrier| BattleShip| Cruiser|Submarine | Destroyer
struct PlayerPieces* setPlayerPieces(struct PlayerPieces* pp) {
    pp = (struct PlayerPieces*) malloc(number_of_Piece*sizeof(struct PlayerPieces));

    int i;

    for(i=0;i<number_of_Piece;i++) {
        if (i==3) {
            pp[i].max_num_of_piece = 2;
            pp[i].num_of_single_piece = 0;
            pp[i].s_piece = setPiece(i+1);
        }
        else {
            pp[i].max_num_of_piece = 1;
            pp[i].num_of_single_piece = 0;
            pp[i].s_piece = setPiece(i+1);
        }
    }
    return pp;
}

struct Player* setPlayer(int* num_of_Players) {
    struct Player* t_Player = (struct Player*) malloc(sizeof(struct Player));

    t_Player->pieces = setPlayerPieces(t_Player->pieces);

    t_Player->num_of_Player = *num_of_Players;
    t_Player->score = 0;

    t_Player->GameBoard = createBoard();
    t_Player->AttackBoard = createBoard();

    *num_of_Players = *num_of_Players + 1;
    return t_Player;
}

void remainedPieces(struct Player* p) {
    int i;
    printf("\nCurrent Pieces Available: ");

    for(i=0;i<number_of_Piece;i++) {
        if (p->pieces[i].s_piece->type == 1) {
            printf("\nCarrier remained %d maximum amount %d.",p->pieces[i].num_of_single_piece, p->pieces[i].max_num_of_piece);
        }
        else if (p->pieces[i].s_piece->type == 2) {
            printf("\nBattleShip remained %d maximum amount %d.",p->pieces[i].num_of_single_piece, p->pieces[i].max_num_of_piece);
        }
        else if (p->pieces[i].s_piece->type == 3) {
            printf("\nCruiser remained %d maximum amount %d.",p->pieces[i].num_of_single_piece, p->pieces[i].max_num_of_piece);
        }
        else if (p->pieces[i].s_piece->type == 4) {
            printf("\nSubmarine remained %d maximum amount %d.",p->pieces[i].num_of_single_piece, p->pieces[i].max_num_of_piece);
        }
        else if (p->pieces[i].s_piece->type == 5) {
            printf("\nDestroyer remained %d maximum amount %d.",p->pieces[i].num_of_single_piece, p->pieces[i].max_num_of_piece);
        }
    }

    printf("\n\n\n");

}

void displayPlayer(struct Player* p) {
    int i;
    printf("Player %d statistic: \n",p->num_of_Player);
    printf("Current Pieces Available: \n");

    for(i=0;i<number_of_Piece;i++) {
        if (p->pieces[i].s_piece->type == 1) {
            printf("\nCarrier:\n");
            printf("Have %d placed from %d maximum amount.\n",p->pieces[i].num_of_single_piece, p->pieces[i].max_num_of_piece);
            printf("Size of Carrier: %d\tType of Carrier: ",p->pieces[i].s_piece->number_of_holes);
            displaySpecialSkill(p->pieces[i].s_piece->specialSkill);
        }
        else if (p->pieces[i].s_piece->type == 2) {
            printf("\nBattleShip:\n");
            printf("Have %d placed from %d maximum amount.\n",p->pieces[i].num_of_single_piece, p->pieces[i].max_num_of_piece);
            printf("Size of BattleShip: %d\tType of BattleShip: ",p->pieces[i].s_piece->number_of_holes);
            displaySpecialSkill(p->pieces[i].s_piece->specialSkill);
        }
        else if (p->pieces[i].s_piece->type == 3) {
            printf("\nCruiser:\n");
            printf("Have %d placed from %d maximum amount.\n",p->pieces[i].num_of_single_piece, p->pieces[i].max_num_of_piece);
            printf("Size of Cruiser: %d\tType of Cruiser: ",p->pieces[i].s_piece->number_of_holes);
            displaySpecialSkill(p->pieces[i].s_piece->specialSkill);
        }
        else if (p->pieces[i].s_piece->type == 4) {
            printf("\nSubmarine:\n");
            printf("Have %d placed from %d maximum amount.\n",p->pieces[i].num_of_single_piece, p->pieces[i].max_num_of_piece);
            printf("Size of Submarine: %d\tType of Submarine: ",p->pieces[i].s_piece->number_of_holes);
            displaySpecialSkill(p->pieces[i].s_piece->specialSkill);
        }
        else if (p->pieces[i].s_piece->type == 5) {
            printf("\nDestroyer:\n");
            printf("Have %d placed from %d maximum amount.\n",p->pieces[i].num_of_single_piece, p->pieces[i].max_num_of_piece);
            printf("Size of Submarine: %d\tType of Submarine: ",p->pieces[i].s_piece->number_of_holes);
            displaySpecialSkill(p->pieces[i].s_piece->specialSkill);
        }
    }

    printf("\n\n\n");
//    printf("Submarine: %d\n",p->num_of_Player);
//    printf("Cruiser: %d\n");
//    printf("Battleship: %d\n");
//    printf("Carrier: %d\n");


}

void tester(int** Board) {
    struct ShipStruct* ship9 = (struct ShipStruct*) malloc(sizeof(struct ShipStruct));

    ship9->type = Battleship;

    printf("Ship 9 has type as %d\n",ship9->type);
}

int** createBoard(void) {
    int** Board;
    int i;

    Board = (int**)malloc(rows*sizeof(int*));

    for(i=0;i<rows;i++){
        *(Board+i) = (int*)calloc(columns,sizeof(int));
    }

    return Board;
}

void printBoard(int **a) {
    int i,j;

    printf("    ");
    for(i=0;i<columns;i++) {
        printf("%d\t",i+1);
    }
    printf("\n    ");
    for(i=0;i<columns;i++) {
        printf("#\t",i+1);
    }
    printf("\n");

    for(i=0;i<rows;i++) {
        printf("%2c #",i+65); // |
        for(j=0;j<columns;j++) {
            printf("%d\t",a[i][j]);
        }
        printf("\n");
    }
}

void end_of_Game(struct Player* player) {
    int i;

    for(i=0;i<rows;i++) {
        free(*((player->GameBoard)+i));
    }
    free(player->GameBoard);

    for(i=0;i<rows;i++) {
        free(*((player->AttackBoard)+i));
    }
    free(player->AttackBoard);

    for(i=0;i<number_of_Piece;i++) {
        free(player->pieces[i].s_piece);
    }

    free(player->pieces);

    free(player);
}
