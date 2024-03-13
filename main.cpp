/*Project Reqs Completed:
(6 Basic Reqs Completed)
Advanced Graphics
Random Generation
Projectiles
Collision
Difficultiy Levels
Artistry*/


#include <FEHLCD.h>
#include <FEHUtility.h>
#include <FEHImages.h>


/*Dungeon Hero
Game designed for FEH Proteus Simulator
Functions & classes written by Connor Radkov
Images made by Guy Levi*/


// Declare player class
class Player {
    public:
        Player(); // Constructor function
        int maxHealth, health, xLocation, yLocation, currentCoins, levelPlus1, speedAdd, currentImage;
};


// Declare enemy class
class Enemy {
    public:
        Enemy(int x, int y, Player* z); // Constructor function
        int xLocation, yLocation, enemyPower, currentImage, fireballDirection, xFBlocation, yFBlocation;
        char state, fireballState;
};


// Declare treasure chest class
class Chest {
    public:
        Chest(); // Constructor function
        int xLocation, yLocation, currentImage;
        char state;
};


// Declare functions
void makeMenu();
void showPlayScreen(Player* y, Chest* z, Enemy* enemy1, Enemy* enemy2);
void showInstructs();
void showCredits();
void showStatsScreen(Player* y);
void makeQuit();
void WinDisplay(Player* y, Chest* z, Enemy* enemy1, Enemy* enemy2);
void LoseDisplay(Player* y, Chest* z, Enemy* enemy1, Enemy* enemy2);
int DrawDungeonMap(Player* y);
void playerWalk(int location, int swordVar, Player* y, Chest* z, Enemy* enemy1, Enemy* enemy2);
void updateScreen(int swordVar, Player* y, Chest* z, Enemy* enemy1, Enemy* enemy2);
void enemyWalk(Enemy* walkEnemy, int swordVar, Player* x, Chest* y, Enemy* otherEnemy);
int swingSword(int swordVar, Player* y, Enemy* enemy1, Enemy* enemy2, Chest* z);
void drawChest(Chest* z);
void drawEnemy(Enemy* x);
void drawControls();
void drawPlayer(Player* x);
void enemySprite(Enemy* walkEnemy, int swordVar, Player* MP, Chest* treasureChest, Enemy* otherEnemy); // enemy walking sprite
void enemyCycle(Enemy* walkEnemy, int swordVar, Player* MP, Chest* treasureChest, Enemy* otherEnemy); // Cycle through enemy images to create sprite
void drawFloorPic();
void playerWalking(int swordVar, Player* MP, Chest* treasureChest, Enemy* enemy1, Enemy* enemy2);
void playerSprite(int swordVar, Player* MP, Chest* treasureChest, Enemy* enemy1, Enemy* enemy2);
void chestAnimate(int swordVar, Player* MP, Chest* treasureChest, Enemy* enemy1, Enemy* enemy2);
void swordAnimation(int swordVar, Player* MP, Chest* treasureChest, Enemy* enemy1, Enemy* enemy2);
void shootFireball(Enemy* walkEnemy, int swordVar, Player* MP, Chest* treasureChest, Enemy* otherEnemy);
void checkDmg(Enemy* walkEnemy, Player* MP);
 
int main(void) {


    // Declare variables
    float xClick, yClick;
    int x, y;
    Player mainPlayer;
    Player* playerPtr = &mainPlayer;
    Chest treasure;
    Chest* chestPtr = &treasure;
    x = rand() % (148 - 20 + 1) + 20;
    y = rand() % (100+1);
    Enemy enemy1(x, y, playerPtr);
    Enemy* enemy1Ptr = &enemy1;
    x = rand() % (148 - 20 + 1) + 20;
    y = rand() % (100+1);
    Enemy enemy2(x, y, playerPtr);
    Enemy* enemy2Ptr = &enemy2;


    while(1) {


        // Draw menu
        makeMenu();


        // Clear previous clicks
        LCD.ClearBuffer();


        // Wait until user clicks screen    
        while(!LCD.Touch(&xClick, &yClick));
        while(LCD.Touch(&xClick, &yClick));


        // Evaluate user click location and display appropriate screen
        if (xClick < 150 && 60 < yClick && yClick < 130) {
            showPlayScreen(playerPtr, chestPtr, enemy1Ptr, enemy2Ptr);
        } else if (xClick < 150 && yClick > 130 && yClick < 180) {
            showInstructs();
        } else if (xClick > 150 && yClick > 110 && yClick < 180) {
            showCredits();
        } else if (xClick > 150 && yClick < 110 && 60 < yClick) {
            showStatsScreen(playerPtr);
        }
    }
}


void makeMenu() {


    // Clear previous clicks
    LCD.ClearBuffer();


    // Clear Display
    LCD.Clear();

    // Draw floor
    FEHImage floor;
    floor.Open("backgroundFEH.pic");
    floor.Draw(0,0);
    floor.Close();

    // Display image
    FEHImage titleImage;
    titleImage.Open("Dungeon_HeroFEH.pic");
    titleImage.Draw(50, 5);
    titleImage.Close();

    // Draw start button
    FEHImage START;
    START.Open("STARTFEH.pic");
    START.Draw(40, 90);
    START.Close();
   
   // Draw stats button
    FEHImage STATS;
    STATS.Open("STATSFEH.pic");
    STATS.Draw(181, 80);
    STATS.Close();

    // DDraw instructions button
    FEHImage INSTRUCTIONS;
    INSTRUCTIONS.Open("INSTRUCTIONSFEH.pic");
    INSTRUCTIONS.Draw(20, 150);
    INSTRUCTIONS.Close();
   
    // Draw credits button
    FEHImage CREDITS;
    CREDITS.Open("CREDITSFEH.pic");
    CREDITS.Draw(200, 150);
    CREDITS.Close();

    // update screen
    LCD.Update();
}


// Display play screen
void showPlayScreen(Player* MP, Chest* treasureChest, Enemy* enemy1, Enemy* enemy2) {


    // Declare variables
    int roomCount = 0, xTouch, yTouch, x, y, swordVar = 0;


    // Run until player wins or loses
    while(1) {


        MP->health = MP->maxHealth; // Set health of player to max health
        DrawDungeonMap(MP); // Generate map
        drawChest(treasureChest); // Create chest


        // Create enemies and set power based on level
        drawEnemy(enemy1);
        drawEnemy(enemy2);
        enemy1->enemyPower = 20 + 10 * (MP->levelPlus1 - 2);
        enemy2->enemyPower = 20 + 10 * (MP->levelPlus1 - 2);


        // Run while player health is above zero and player has not reached the end
        while(MP->health > 0 && roomCount < MP->levelPlus1) {


            // Wait for user click
            while(!LCD.Touch(&xTouch, &yTouch));
            while(LCD.Touch(&xTouch, &yTouch));


            // Evaluate where user clicked
            if (267 < xTouch && xTouch < 282 && 181 < yTouch && yTouch < 196) { // Up
                // See which enemies still exist
                if (enemy1->state == 'A') {
                    enemyWalk(enemy1, swordVar, MP, treasureChest, enemy2);
                }
                if (enemy2->state == 'A') {
                    enemyWalk(enemy2, swordVar, MP, treasureChest, enemy1);
                }
                // Call player walk function, passing 0, if user is not at max height
                if (MP->yLocation > 0) {
                    playerWalk(0, swordVar, MP, treasureChest, enemy1, enemy2);
                }
            } else if (267 < xTouch && xTouch < 282 && 221 < yTouch && yTouch < 236) { // Down
                // See which enemies still exist
                if (enemy1->state == 'A') {
                    enemyWalk(enemy1, swordVar, MP, treasureChest, enemy2);
                }
                if (enemy2->state == 'A') {
                    enemyWalk(enemy2, swordVar, MP, treasureChest, enemy1);
                }
                // Call player walk function, passing 1, if user is not at min height
                if ((MP->yLocation + MP->speedAdd) < 210) {
                    playerWalk(1, swordVar, MP, treasureChest, enemy1, enemy2);
                }
            } else if (247 < xTouch && xTouch < 263 && 201 < yTouch && yTouch < 216) { // Left
                // See which enemies still exist
                if (enemy1->state == 'A') {
                    enemyWalk(enemy1, swordVar, MP, treasureChest, enemy2);
                }
                if (enemy2->state == 'A') {
                    enemyWalk(enemy2, swordVar, MP, treasureChest, enemy1);
                }
                // Call player walk function, passing 2, if user is not at min width
                if ((MP->xLocation - MP->speedAdd)> 20) {
                    playerWalk(2, swordVar, MP, treasureChest, enemy1, enemy2);
                }
            } else if (287 < xTouch && xTouch < 303 && 201 < yTouch && yTouch < 216) { // Right
                // See which enemies still exist
                if (enemy1->state == 'A') {
                    enemyWalk(enemy1, swordVar, MP, treasureChest, enemy2);
                }
                if (enemy2->state == 'A') {
                    enemyWalk(enemy2, swordVar, MP, treasureChest, enemy1);
                }
                // Call player walk function, passing 3, if user is not at max width
                if ((MP->xLocation + MP->speedAdd) < 200) {
                    playerWalk(3, swordVar, MP, treasureChest, enemy1, enemy2);
                }
            } else if (267 < xTouch && xTouch < 282 && 201 < yTouch && yTouch < 216) { // Sword
                // Player swinging sword animation
                swordAnimation(swordVar, MP, treasureChest, enemy1, enemy2);
                // Call SwingSword function
                swordVar = swingSword(swordVar, MP, enemy1, enemy2, treasureChest);
                // Update screen
                updateScreen(swordVar, MP, treasureChest, enemy1, enemy2);
            }


            // Player reaches next room
            if (MP->yLocation <= (0+MP->speedAdd+20)) {
                // Clear, add 1 to roomCount, reset swordVar, regen enemies / chest and create next room
                LCD.Clear();
                roomCount++;
                swordVar = 0;
                treasureChest->currentImage = 0;
                treasureChest->state = 'A';
                enemy1->state = 'A';
                enemy2->state= 'A';
                enemy1->xLocation = rand() % (148 - 20 + 1) + 20;
                enemy1->yLocation = rand() % (100+1);
                enemy2->xLocation = rand() % (148 - 20 + 1) + 20;
                enemy2->yLocation = rand() % (100+1);
                treasureChest->xLocation = rand() % (180-20+1) + 20;
                treasureChest->yLocation = rand() % (160-10+1) + 10;
                DrawDungeonMap(MP);
                drawChest(treasureChest);
                drawEnemy(enemy1);
                drawEnemy(enemy2);
            }
        }


        // Evaluate if player won or lost the level
        if(MP->health <=0) {
            LoseDisplay(MP, treasureChest, enemy1, enemy2);
            break;
        } else if (roomCount = MP->levelPlus1) {
            MP->levelPlus1 ++;
            WinDisplay(MP, treasureChest, enemy1, enemy2);
            break;
        }
    }
}


// Display instructions
void showInstructs() {


    // Declare variables
    char quit = 'n';
    float xTouch, yTouch;


    // Clear previous display
    LCD.Clear();

    // Draw floor
    FEHImage floor;
    floor.Open("backgroundFEH.pic");
    floor.Draw(0,0);
    floor.Close();

    // Display the instructions
    LCD.SetFontColor(WHITE);
    LCD.WriteLine("Instructions:");
    LCD.WriteLine("Navigate through the dungeon!");
    LCD.WriteLine("Collect coins from chest to upgrade stats!");
    LCD.WriteLine("Kill monsters or they will kill you!");
    LCD.Update();
    Sleep(0.5);


    // Create quit button
    makeQuit();


    // Wait for user to exit via the quit button
    while (quit = 'n') {


        Sleep(0.001);


        while(!LCD.Touch(&xTouch, &yTouch));
        while(LCD.Touch(&xTouch, &yTouch));


        // Check if user clicked the quit button. If so, quit
        if (100 < xTouch && xTouch < 225 && 150 < yTouch && yTouch < 225) {
            LCD.Clear();
            break;
        }
    }
}


// Display credits
void showCredits() {


    // Declare variables
    char quit = 'n';
    float xTouch, yTouch;
   
    // Clear previous display
    LCD.Clear();

    // Draw floor
    FEHImage floor;
    floor.Open("backgroundFEH.pic");
    floor.Draw(0,0);
    floor.Close();

    // Display credits
    LCD.SetFontColor(WHITE);
    LCD.WriteLine("Created by:");
    LCD.WriteLine("Guy Levi & Connor Radkov");
    LCD.WriteLine("Created using:");
    LCD.WriteLine("OSU FEH Proteus Sim.");
    LCD.Update();
    Sleep(0.5);


    // Create quit button
    makeQuit();


    // Wait for user to exit via the quit button
    while (quit = 'n') {


        Sleep(0.001);


        while(!LCD.Touch(&xTouch, &yTouch));
        while(LCD.Touch(&xTouch, &yTouch));


        // Check if user clicked the quit button. If so, quit
        if (100 < xTouch && xTouch < 225 && 150 < yTouch && yTouch < 225) {
            LCD.Clear();
            break;
        }
    }
}


// Display stats screen
void showStatsScreen(Player* MP) {


    // Declare variables
    float xTouch, yTouch;


    // Clear previous display
    LCD.Clear();

    // Draw floor
    FEHImage floor;
    floor.Open("backgroundFEH.pic");
    floor.Draw(0,0);
    floor.Close();

    // Display the stats screen
    LCD.SetFontColor(WHITE);
    LCD.WriteLine("Player Speed Level:");
    LCD.WriteLine(MP->speedAdd);
    LCD.WriteLine("Player Health Level:");
    LCD.WriteLine(MP->maxHealth);
    LCD.WriteLine("Coins:");
    LCD.WriteLine(MP->currentCoins);
    LCD.SetFontColor(YELLOW);
    LCD.WriteLine("Upgrade for 5 coins");


    // Make upgrade buttons
    LCD.SetFontColor(BLACK);
    LCD.DrawRectangle(260, 5, 20, 20);
    LCD.DrawRectangle(260, 35, 20, 20);
    LCD.FillRectangle(260, 5, 20, 20);
    LCD.FillRectangle(260, 35, 20, 20);
    LCD.SetFontColor(WHITE);
    LCD.WriteAt("+", 263, 8);
    LCD.WriteAt("+", 263, 38);


    // Create quit button
    makeQuit();


    // Wait for user to click
    while (1) {


        Sleep(0.001);


        while(!LCD.Touch(&xTouch, &yTouch));
        while(LCD.Touch(&xTouch, &yTouch));


        // Check if user clicked a button
        if (100 < xTouch && xTouch < 225 && 150 < yTouch && yTouch < 225) { // User clicks quit
            LCD.Clear();
            break;
        } else if (260 < xTouch && xTouch < 280 && 5 < yTouch && yTouch < 25) { // User upgrades speed
            if (MP->currentCoins >= 5) { // See if user has enough coins to upgrade speed
                MP->speedAdd += 1;
                MP->currentCoins -= 5;
                showStatsScreen(MP);
                break;
            }
        } else if (260 < xTouch && xTouch < 280 && 35 < yTouch && yTouch < 55) { // User upgrades health
            if (MP->currentCoins >= 5) { // See if user has enough coins to upgrade health
                MP->maxHealth += 5;
                MP->currentCoins -= 5;
                showStatsScreen(MP);
                break;
            }
        }
    }
}


// Create quit button
void makeQuit() {


    FEHImage quit;
    quit.Open("quit3FEH.pic");
    quit.Draw(115,170);
    quit.Close();
}


// Chest Constructor
Chest::Chest() {


    // Generate a coordinate pair and create a chest there
    xLocation = rand() % (180-20+1) + 20;
    yLocation = rand() % (160-10+1) + 10;
    currentImage = 0;
    state = 'A';
}


// Display win screen
void WinDisplay(Player* MP, Chest* treasureChest, Enemy* enemy1, Enemy* enemy2) {


    // Clear previous display
    LCD.Clear();


    // Declare variables
    float xTouch, yTouch;

    while(1) {

        // Draw floor
        FEHImage floor;
        floor.Open("backgroundFEH.pic");
        floor.Draw(0,0);
        floor.Close();

        // Write win statement and create buttons
        FEHImage win;
        win.Open("You_winFEH.pic");
        win.Draw(50,80);
        win.Close();

        FEHImage NxtLvl;
        NxtLvl.Open("next levelFEH.pic");
        NxtLvl.Draw(25, 170);
        NxtLvl.Close();

        FEHImage ChkSts;
        ChkSts.Open("STATSFEH.pic");
        ChkSts.Draw(130, 200);
        ChkSts.Close();

        FEHImage Quit_g;
        Quit_g.Open("quit_igFEH.pic");
        Quit_g.Draw(225, 190);
        Quit_g.Close();
        

        // Wait for user click
        while(!LCD.Touch(&xTouch, &yTouch));
        while(LCD.Touch(&xTouch, &yTouch));


        // Evaluate user click
        if (xTouch < 100 && xTouch > 20 && yTouch < 240 && yTouch > 200) {
            // Take user to next level
            showPlayScreen(MP, treasureChest, enemy1, enemy2);
            break;
        } else if (xTouch < 200 && xTouch > 120 && yTouch < 240 && yTouch > 200) {
            // Take user to upgrade stats screen
            showStatsScreen(MP);
        } else if (xTouch < 300 && xTouch > 220 && yTouch < 240 && yTouch > 200) {
            // Take user to menu
            LCD.Clear();
            break;
        }
    }
}


// Display lose screen
void LoseDisplay(Player* MP, Chest* treasureChest, Enemy* enemy1, Enemy* enemy2) {


    // Clear previous display
    LCD.Clear();

    // Declare variables
    float xTouch, yTouch;


    while(1) {
        
        // Draw floor
        FEHImage floor;
        floor.Open("backgroundFEH.pic");
        floor.Draw(0,0);
        floor.Close();

        FEHImage loose;
        loose.Open("You_diedFEH.pic");
        loose.Draw(40,80);
        loose.Close();

        FEHImage NxtLvl;
        NxtLvl.Open("retryFEH.pic");
        NxtLvl.Draw(25, 170);
        NxtLvl.Close();

        FEHImage ChkSts;
        ChkSts.Open("STATSFEH.pic");
        ChkSts.Draw(130, 200);
        ChkSts.Close();

        FEHImage Quit_g;
        Quit_g.Open("quit_igFEH.pic");
        Quit_g.Draw(225, 190);
        Quit_g.Close();

        // Wait for user click
        while(!LCD.Touch(&xTouch, &yTouch));
        while(LCD.Touch(&xTouch, &yTouch));


        // Evaluate user click
        if (xTouch < 100 && xTouch > 20 && yTouch < 240 && yTouch > 200) {
            // Let user repeat level
            // Reset key stats
            treasureChest->currentImage = 0;
            treasureChest->state = 'A';
            enemy1->state = 'A';
            enemy2->state= 'A';
            enemy1->xLocation = rand() % (148 - 20 + 1) + 20;
            enemy1->yLocation = rand() % (100+1);
            enemy2->xLocation = rand() % (148 - 20 + 1) + 20;
            enemy2->yLocation = rand() % (100+1);
            treasureChest->xLocation = rand() % (180-20+1) + 20;
            treasureChest->yLocation = rand() % (160-10+1) + 10;
            showPlayScreen(MP, treasureChest, enemy1, enemy2);
            break;
        } else if (xTouch < 200 && xTouch > 120 && yTouch < 240 && yTouch > 200) {
            // Take user to upgrade stats screen
            showStatsScreen(MP);
        } else if (xTouch < 300 && xTouch > 220 && yTouch < 240 && yTouch > 200) {
            // Take user to menu
            LCD.Clear();
            break;
        }
    }
}


// Draw Dunegon Function
int DrawDungeonMap(Player* MP) {


    // Reset player location
    MP->xLocation = 140;
    MP->yLocation = 200;


    // Declare variables
    int x, y;


    // Clear previous screen
    LCD.Clear();


    // Draw Dungeon floor & walls
    drawFloorPic();


    // Display the player + player stats
    drawPlayer(MP);


    // Display touchpad
    drawControls();


    return 0;
}


// Player swings sword function
int swingSword(int swordVar, Player* MP, Enemy* enemy1, Enemy* enemy2, Chest* treasureChest) {
    // Chests drop 1-5 coins, killing enemy gives 5 coins
    // Evaluate if the user is facing any object within an appropriate distance
    /*User's x value and y value is close to enemy 1*/
    if ( ((enemy1->xLocation - 48) <= MP->xLocation && MP->xLocation <= (enemy1->xLocation + 48)) && ((enemy1->yLocation - 54) <= MP->yLocation && MP->yLocation <= (enemy1->yLocation + 54)) && enemy1->state == 'A') {
            if (swordVar == 0) { // If all 3 objects are present
                MP->currentCoins += 5;
                enemy1->state = 'D';
                return 3;
            } else if (swordVar == 1) { // If E1 & E2 are present
                MP->currentCoins += 5;
                enemy1->state = 'D';
                return 6;
            } else if (swordVar == 2) { // If E1 & chest are present
                MP->currentCoins += 5;
                enemy1->state = 'D';
                return 4;
            } else if (swordVar == 5) { // if E1 is present
                MP->currentCoins += 5;
                enemy1->state = 'D';
                return 7;
            }
    /*User's x value and y value is close to enemy 2*/
    } else if ( ((enemy2->xLocation - 48) <= MP->xLocation && MP->xLocation <= (enemy2->xLocation + 48)) && ((enemy2->yLocation - 54) <= MP->yLocation && MP->yLocation <= (enemy2->yLocation + 54)) && enemy2->state == 'A') {
        if (swordVar == 0) { // If all 3 objects are present
            MP->currentCoins += 5;
            enemy2->state = 'D';
            return 2;
        } else if (swordVar == 1) { // If E1 & E2 are present
            MP->currentCoins += 5;
            enemy2->state = 'D';
            return 5;
        } else if (swordVar == 3) { // If E2 & Chest are present
            MP->currentCoins += 5;
            enemy2->state = 'D';
            return 4;
        } else if (swordVar == 6) { // If E2 is present
            MP->currentCoins += 5;
            enemy2->state = 'D';
            return 7;
        }
    /*User's x value and y value is close to treasureChest*/
    } else if ( ((treasureChest->xLocation - 20) <= MP->xLocation && MP->xLocation <= (treasureChest->xLocation + 20)) && ((treasureChest->yLocation - 30) <= MP->yLocation && MP->yLocation <= (treasureChest->yLocation + 30)) && treasureChest->state == 'A') {
        if (swordVar == 0) { // If all 3 objects are present, give user a random number of coins 1 - 5
            chestAnimate(swordVar, MP, treasureChest, enemy1, enemy2);
            MP->currentCoins += rand() % (10-5+1) + 5;
            treasureChest->state = 'D';
            return 1;
        } else if (swordVar == 2) { // If E1 & Chest are present, give user a random number of coins 1 - 5
            chestAnimate(swordVar, MP, treasureChest, enemy1, enemy2);
            MP->currentCoins += rand() % (10-5+1) + 5;
            treasureChest->state = 'D';
            return 5;
        } else if (swordVar == 3) { // If E2 and Chest are present, give user a random number of coins 1 - 5
            chestAnimate(swordVar, MP, treasureChest, enemy1, enemy2);
            MP->currentCoins += rand() % (10-5+1) + 5;
            treasureChest->state = 'D';
            return 6;
        } else if (swordVar == 4) { // If Chest is present, give user a random number of coins 1 - 5
            chestAnimate(swordVar, MP, treasureChest, enemy1, enemy2);
            MP->currentCoins += rand() % (10-5+1) + 5;
            treasureChest->state = 'D';
            return 7;
        }
    } else {
        return swordVar; // Missed strike
    }
}


// Display screen after user kills something with sword
void updateScreen(int swordVar, Player* MP, Chest* treasureChest, Enemy* enemy1, Enemy* enemy2) {


    // Clear screen
    LCD.Clear();


    // Draw Dungeon floor & walls
    drawFloorPic();


    // Redraw touchpad
    drawControls();
    

    // Draw objects
    if (enemy1->state == 'A') {
        drawEnemy(enemy1);
    }
    if (enemy2->state == 'A') {
        drawEnemy(enemy2);
    }
    drawChest(treasureChest);

    // Redraw the player + player stats
    drawPlayer(MP);


    // Draw fireball, if it exists
    if (enemy1->fireballState == 'A') {
       FEHImage fireball;
       fireball.Open("fireballFEH.pic");
       fireball.Draw(enemy1->xFBlocation, enemy1->yFBlocation);
       fireball.Close();
    } else if (enemy2->fireballState == 'A') {
       FEHImage fireball2;
       fireball2.Open("fireballFEH.pic");
       fireball2.Draw(enemy2->xFBlocation, enemy2->yFBlocation);
       fireball2.Close();
    }
}


// Allow user to upgrade stats
void upgradeStats(Player* MP) {
    // Clear screen
    LCD.Clear();


    //Display options to user
    LCD.WriteAt("Current Health:", 20, 15);
    LCD.WriteAt(MP->health, 20, 25);
    LCD.WriteAt("Current Speed Boost:", 20, 45);
    LCD.WriteAt(MP->speedAdd, 20, 65);
    LCD.WriteAt("Current Coins:", 20, 85);
    LCD.WriteAt(MP->currentCoins, 20, 105);
}


// Draw controls
void drawControls() {

    FEHImage Up;

    Up.Open("Button_uFEH.pic");
    Up.Draw(267, 181);
    Up.Close();

    FEHImage Down;
    Down.Open("Button_dFEH.pic");
    Down.Draw(267, 221);
    Down.Close();

    FEHImage Right;
    Up.Open("Button_rFEH.pic");
    Up.Draw(287, 201);
    Up.Close();

    FEHImage Left;
    Up.Open("Button_lFEH.pic");
    Up.Draw(247, 201);
    Up.Close();

    FEHImage Sword;
    Up.Open("Button_sFEH.pic");
    Up.Draw(267, 201);
    Up.Close();
}


// Determine which frame of the enemy walking we are on
void enemySprite (Enemy* walkEnemy, int swordVar, Player* MP, Chest* treasureChest, Enemy* otherEnemy) {


    // Begin cycling through images
    Sleep(0.01);
    // Change enemy image and update screen
    walkEnemy->currentImage ++;        
    updateScreen(swordVar, MP, treasureChest, walkEnemy, otherEnemy);
}


void enemyCycle(Enemy* walkEnemy, int swordVar, Player* MP, Chest* treasureChest, Enemy* otherEnemy) {
   
    // Begin cycling through images
    for (int i = 0; i <= 12; i++) {
        enemySprite(walkEnemy, swordVar, MP, treasureChest, otherEnemy); // Image (i+1)
    }


    // End program, set image back to default
    walkEnemy->currentImage = 0;
    updateScreen(swordVar, MP, treasureChest, walkEnemy, otherEnemy);
}


// Enemy Constructor
Enemy::Enemy(int x, int y, Player* MP) {


    // Set default power value
    enemyPower = 20;


    // Store the location of enemy
    xLocation = x;
    yLocation = y;


    // Set image to image 1
    currentImage = 0;

    // Set state and fireball state
    state = 'A';
    fireballState = 'D';
}


// Make enemy move 5 spaces in a random direction
void enemyWalk(Enemy* walkEnemy, int swordVar, Player* MP, Chest* treasureChest, Enemy* otherEnemy) {


    // Generate random number 0-3
    int x;
    x = rand() % (3+1);


    // Move corresponding direction
    if (x == 0) {
        if (walkEnemy->yLocation > 25) {
            // Move enemy if not at max height
            walkEnemy->yLocation -= 10;
        }
    } else if (x == 1) {
        if (walkEnemy->yLocation < 220) {
            // Move enemy if not at min height
            walkEnemy->yLocation += 10;
        }
    } else if (x == 2) {
        if (walkEnemy->xLocation > 20) {
            // Move enemy if not at min width
            walkEnemy->xLocation -= 10;
        }
    } else if (x == 3) {
        if (walkEnemy->xLocation < 148) {
            // Move enemy if not at max width
            walkEnemy->xLocation += 10;
        }
    }

    // Enemy walk animation
    enemyCycle(walkEnemy, swordVar, MP, treasureChest, otherEnemy);
    // Make enemy shoot fireball
    shootFireball(walkEnemy, swordVar, MP, treasureChest, otherEnemy);
}


// Make enemy shoot fireball
void shootFireball(Enemy* walkEnemy, int swordVar, Player* MP, Chest* treasureChest, Enemy* otherEnemy) {
    
    // Make fireball state alive and set locations
    walkEnemy->fireballState = 'A';
    walkEnemy->xFBlocation = walkEnemy->xLocation + 35;
    walkEnemy->yFBlocation = walkEnemy->yLocation + 30;

    // Choose a random direction for the fireball to shoot
    walkEnemy->fireballDirection =  rand() % (8-1+1) + 1;
    
    // Determine which direction the fireball shoots. Keep it moving until it hits a wall or the player
    if (walkEnemy->fireballState == 1) { // Up
        while(walkEnemy->yFBlocation > 5 && walkEnemy->fireballState == 'A') { // while it has not hit a wall or the player
            walkEnemy->yFBlocation --;
            Sleep(0.001);
            checkDmg(walkEnemy, MP);
            updateScreen(swordVar, MP, treasureChest, walkEnemy, otherEnemy);
        } 
    } else if (walkEnemy->fireballDirection == 2) { // Up and right
        while(walkEnemy->xFBlocation < 205 && walkEnemy->yFBlocation > 5 && walkEnemy->fireballState == 'A') { // while it has not hit a wall or the player
            walkEnemy->yFBlocation --;
            walkEnemy->xFBlocation ++;
            Sleep(0.001);
            checkDmg(walkEnemy, MP);
            updateScreen(swordVar, MP, treasureChest, walkEnemy, otherEnemy);
        }
    } else if (walkEnemy->fireballDirection == 3) { // Right
        while(walkEnemy->xFBlocation < 205 && walkEnemy->fireballState == 'A') { // white it has not hit a wall or the player
            walkEnemy->xFBlocation ++;
            Sleep(0.001);
            checkDmg(walkEnemy, MP);
            updateScreen(swordVar, MP, treasureChest, walkEnemy, otherEnemy);
        }
    } else if (walkEnemy->fireballDirection == 4){ // Down and right
        while(walkEnemy->xFBlocation < 205 && walkEnemy->yFBlocation < 235 && walkEnemy->fireballState == 'A') { // while it has not hit a wall or the player
            walkEnemy->xFBlocation++;
            walkEnemy->yFBlocation++;
            Sleep(0.001);
            checkDmg(walkEnemy, MP);
            updateScreen(swordVar, MP, treasureChest, walkEnemy, otherEnemy);
        }
    } else if (walkEnemy->fireballDirection == 5) { // Down
        while(walkEnemy->yFBlocation < 235 && walkEnemy->fireballState == 'A') { // while it has not hit a wall or the player
            walkEnemy->yFBlocation ++;
            Sleep(0.001);
            checkDmg(walkEnemy, MP);
            updateScreen(swordVar, MP, treasureChest, walkEnemy, otherEnemy);
        }
    } else if (walkEnemy->fireballDirection == 6) { // Down and left
        while(walkEnemy->xFBlocation > 25 && walkEnemy->yFBlocation < 235 && walkEnemy->fireballState == 'A') { // while it has not hit a wall or the player
            walkEnemy->xFBlocation --;
            walkEnemy->yFBlocation ++;
            Sleep(0.001);
            checkDmg(walkEnemy, MP);
            updateScreen(swordVar, MP, treasureChest, walkEnemy, otherEnemy);
        }
    } else if (walkEnemy->fireballDirection == 7) { // Left
        while(walkEnemy->xFBlocation > 25 && walkEnemy->fireballState == 'A') { // while it has not hit a wall or the plaeyr
            walkEnemy->xFBlocation --;
            Sleep(0.001);
            checkDmg(walkEnemy, MP);
            updateScreen(swordVar, MP, treasureChest, walkEnemy, otherEnemy);
        }
    } else if (walkEnemy->fireballDirection == 8) { // Left and up
        while(walkEnemy->xFBlocation > 25 && walkEnemy->yFBlocation > 5 && walkEnemy->fireballState == 'A') { // while it has not hit a wall or the player
            walkEnemy->xFBlocation --;
            walkEnemy->yFBlocation --;
            Sleep(0.001);
            checkDmg(walkEnemy, MP);
            updateScreen(swordVar, MP, treasureChest, walkEnemy, otherEnemy);
        }
    }

    // Set fireball state to dead
    walkEnemy->fireballState = 'D';
}


// Check if fireball hit player
void checkDmg(Enemy* walkEnemy, Player* MP) {
    // Check location of fireball and player
    if ((walkEnemy->xFBlocation < MP->xLocation + 40 && walkEnemy->xFBlocation > MP->xLocation - 20) && (walkEnemy->yFBlocation > MP->yLocation - 20 && walkEnemy->yFBlocation < MP->yLocation + 40)) {
        MP->health = MP->health - walkEnemy->enemyPower; // Deal dmg
        walkEnemy->fireballState = 'D'; // Kill fireball
    }
}


// Draw enemy
void drawEnemy(Enemy* enemy) {
    FEHImage mage;
    mage.Open((("mage_guardian-red(" + std::to_string(enemy->currentImage + 1) + ")FEH.pic")).c_str());
    mage.Draw(enemy->xLocation, enemy->yLocation);
    mage.Close();
}


// Draw floor function
void drawFloorPic() {
    FEHImage Background;
    Background.Open("dungeon_flooring(1)FEH.pic");
    Background.Draw(20,0);
    Background.Close();

    FEHImage Gbackground_L;
    Gbackground_L.Open("game_back_LFEH.pic");
    Gbackground_L.Draw(0,0);
    Gbackground_L.Close();

    FEHImage Gbackground_R;
    Gbackground_R.Open("game_back_RFEH.pic");
    Gbackground_R.Draw(220,0);
    Gbackground_R.Close();

    LCD.SetFontColor(WHITE);
    LCD.DrawVerticalLine(20, 0, 240);
    LCD.DrawVerticalLine(220, 0, 240);
}


// Player Constructor
Player::Player() {
   
    // Set default player values
    health = 100;
    xLocation = 140;
    yLocation = 200;
    currentCoins = 10;
    levelPlus1 = 2;
    speedAdd = 0;
    maxHealth = 100;
    currentImage = 0;
}


// Draw player + player stats
void drawPlayer(Player* MP) {


    // Evaluate which player frame needs to be drawn
    if (MP->currentImage == 0) { // Default standing player
        FEHImage MainPlayer;
        MainPlayer.Open("player(1)FEH.pic");
        MainPlayer.Draw(MP->xLocation, MP->yLocation);
        MainPlayer.Close();
    } else if (1 <= MP->currentImage && MP->currentImage <= 6) { // Player walking left
        FEHImage MainPlayer;
        MainPlayer.Open(("player_l(" + std::to_string(MP->currentImage) + ")FEH.pic").c_str());
        MainPlayer.Draw(MP->xLocation, MP->yLocation);
        MainPlayer.Close();
    } else if (7 <= MP->currentImage && MP->currentImage <= 12) { // Player walking up
        FEHImage MainPlayer;
        MainPlayer.Open(("player_f(" + std::to_string(MP->currentImage - 6) + ")FEH.pic").c_str());
        MainPlayer.Draw(MP->xLocation, MP->yLocation);
        MainPlayer.Close();
    } else if (13 <= MP->currentImage && MP->currentImage <= 18) { // Player walking down
        FEHImage MainPlayer;
        MainPlayer.Open(("player(" + std::to_string(MP->currentImage - 12) + ")FEH.pic").c_str());
        MainPlayer.Draw(MP->xLocation, MP->yLocation);
        MainPlayer.Close();
    } else if (19 <= MP->currentImage && MP->currentImage <= 24) { // Player walking right
        FEHImage MainPlayer;
        MainPlayer.Open(("player_r(" + std::to_string(MP->currentImage - 18) + ")FEH.pic").c_str());
        MainPlayer.Draw(MP->xLocation, MP->yLocation);
        MainPlayer.Close();
    } else if (24 <= MP->currentImage && MP->currentImage <= 29) { // Player swinging sword
        FEHImage MainPlayer;
        MainPlayer.Open(("player_sf(" + std::to_string(MP->currentImage - 23 ) + ")FEH.pic").c_str());
        MainPlayer.Draw(MP->xLocation, MP->yLocation);
        MainPlayer.Close();
    }
   
    // Draw stats
    LCD.SetFontColor(WHITE);
    LCD.WriteAt("Level:", 231, 10);
    LCD.WriteAt(MP->levelPlus1 - 1, 231, 30);
    LCD.WriteAt("Health:", 231, 50);
    LCD.WriteAt(MP->health, 231, 70);
    LCD.WriteAt("Coins:", 231, 90);
    LCD.WriteAt(MP->currentCoins, 231, 110);
}


// Player Walk function
void playerWalk(int direction, int swordVar, Player* MP, Chest* treasureChest, Enemy* enemy1, Enemy* enemy2) {


    // Move player in appropriate direction
        switch (direction){
    case 0:
        MP->yLocation -= (25 + MP->speedAdd); // Up
        MP->currentImage = 6;
        playerWalking(swordVar, MP, treasureChest, enemy1, enemy2);
        break;
    case 1:
        MP->yLocation += (20 + MP->speedAdd); // Down
        MP->currentImage = 12;
        playerWalking(swordVar, MP, treasureChest, enemy1, enemy2);
        break;
    case 2:
        MP->xLocation -= (20 + MP->speedAdd); // Left
        MP->currentImage = 0;
        playerWalking(swordVar, MP, treasureChest, enemy1, enemy2);
        break;
    case 3:
        MP->xLocation += (20 + MP->speedAdd); // Right
        MP->currentImage = 18;
        playerWalking(swordVar, MP, treasureChest, enemy1, enemy2);
        break;
    default:
        break;
        }


    // Update screen
    updateScreen(swordVar, MP, treasureChest, enemy1, enemy2);
}


// Cycle of player walking gif
void playerWalking(int swordVar, Player* MP, Chest* treasureChest, Enemy* enemy1, Enemy* enemy2) {
    // Begin cycling through images
    for (int i = 0; i <= 5; i++) {
        playerSprite(swordVar, MP, treasureChest, enemy1, enemy2);
    }


    // End program, set image back to default
    MP->currentImage = 0;
    updateScreen(swordVar, MP, treasureChest, enemy1, enemy2);
}


// Determine which frame of the player walking we are on
void playerSprite(int swordVar, Player* MP, Chest* treasureChest, Enemy* enemy1, Enemy* enemy2) {


    // Begin cycling through images
    Sleep(0.1);
    // Change player image and update screen
    MP->currentImage ++;
    updateScreen(swordVar, MP, treasureChest, enemy1, enemy2);
}


// Draw chest
void drawChest(Chest* treasureChest) {
    FEHImage chestDraw;
    chestDraw.Open(("chest_0" + std::to_string(treasureChest->currentImage + 1) + "FEH.pic").c_str());
    chestDraw.Draw(treasureChest->xLocation, treasureChest->yLocation);
    chestDraw.Close();
}


// Animation of chest opening
void chestAnimate(int swordVar, Player* MP, Chest* treasureChest, Enemy* enemy1, Enemy* enemy2) {

    // For each image of chest, display image
    for (int i = 0; i <= 2; i++) {
        // Begin cycling through images
        Sleep(0.25);
        // Change chest and update screen
        treasureChest ->currentImage ++;
        updateScreen(swordVar, MP, treasureChest, enemy1, enemy2);
    }
}


// Animation of sword swinging
void swordAnimation(int swordVar, Player* MP, Chest* treasureChest, Enemy* enemy1, Enemy* enemy2) {
    MP->currentImage = 23;
    for (int i = 0; i <= 3; i++) {
        // Begin cycling through images
        Sleep(0.1);
        // Change player and update screen
        MP->currentImage ++;
        updateScreen(swordVar, MP, treasureChest, enemy1, enemy2);
    }

    // Reset player image and update screen
    MP->currentImage = 0;
    updateScreen(swordVar, MP, treasureChest, enemy1, enemy2);
}