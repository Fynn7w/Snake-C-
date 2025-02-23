#include <iostream>
#include <vector>
#include <ncurses.h> // getch(), printw(), refresh(), clear(), curs_set(), noecho(), keypad()
#include <unistd.h> 


enum Direction { UP, DOWN, LEFT, RIGHT };


class food {
    private:
        int x;
        int y;
    
    public:
        food(int _x, int _y){
            this->x = _x;
            this->y = _y;
        }
        
        void random_xy(){
            this->x = rand() % 20;
            this->y = rand() % 20;
        }
        void draw(){
            mvprintw(this->y, this->x, "O");
        }
        int get_x(){
            return this->x;
        }
        int get_y(){
            return this->y;
        }
    };


class Snake {
private:
    std::vector<int> x;
    std::vector<int> y;
    Direction direction;


public:
    Snake(int _x,int _y) {
        this->x.push_back(_x);
        this->y.push_back(_y);
        this->direction = RIGHT;
    }
    int get_x(){
        return this->x[0];
    }
    int get_y(){
        return this->y[0];
    }
    std::vector<int> get_x_segments(){
        return this->x;
    }
    std::vector<int> get_y_segments(){
        return this->y;
    }

    void draw() {
        std::cout<<"x: "<<this->x[0]<<" y: "<<this->y[0];
        for(int i =0;i < this->x.size();i++){
        mvprintw(this->y[i], this->x[i], "#");
        }
    }
    
    void add_segments() {
        this->x.push_back(this->x[this->x.size()-1]);
        this->y.push_back(this->y[this->y.size()-1]);
}

    void move_segments(){
        for(int i = this->x.size()-1; i > 0 ;i--){
            this->x[i] = this->x[i-1];
            this->y[i] = this->y[i-1];
        }
    }


    void move() {
        switch (this->direction) {
            case RIGHT: 
                this->x[0]++; 
                break;
            case LEFT:  
                this->x[0]--; 
                break;
            case UP:    
                this->y[0]--; 
                break;
            case DOWN:  
                this->y[0]++; 
                break;
        }
    }


    void keybindings() {
        int key = getch();
        switch (key) {
            case KEY_UP:    
                if (direction != DOWN) 
                direction = UP; 
                break;
            case KEY_DOWN:  
                if (direction != UP) 
                direction = DOWN; 
                break;
            case KEY_LEFT:  
                if (direction != RIGHT) 
                direction = LEFT; 
                break;
            case KEY_RIGHT: 
                if (direction != LEFT) 
                direction = RIGHT; 
                break;
        }
    }
};


bool collision(int x1,int x2){
    if( x1 == x2){
        return true;
    }
    else{
        return false;
    }
}


void game_over(){
    endwin();
    std::cout<<"Game Over";
    exit(0);
}


void check_gameover(int fx, int fy,std::vector<int> sx,std::vector<int> sy){
    for(int i = 2; i < sx.size();i++){
        if(collision(sx[0],sx[i]) && collision(sy[0],sy[i])){
            game_over();
        }
    }

    if (sx[0] < 0 || sx[0] >= COLS || sy[0] < 0 || sy[0] >= LINES) {
    game_over();
    }
}



void draw_score(int score){
    mvprintw(0,0,"Score: %d",score);
}


void game_loop(){
    int score = 0;    
    initscr();  
    curs_set(0);
    noecho();
    keypad(stdscr, TRUE); 
    Snake snake(10, 10);
    food food(20,20);
    while (true) {
        clear();          
        snake.draw();     
        food.draw();
        draw_score(score);
        refresh();        
        timeout(100);   
        snake.keybindings();
        snake.move();
        snake.move_segments();
        check_gameover(food.get_x(),food.get_y(),snake.get_x_segments(),snake.get_y_segments());
        if(collision(snake.get_x(),food.get_x()) and collision(snake.get_y(),food.get_y())){
            food.random_xy();
            snake.add_segments();   
            score++;
        }
    }
}



int main() {
    game_loop();
    return 0;
   }
