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
                this->add_segments();
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

bool collision(int x1,int x2,int y1,int y2){
    if( x1 ==  x2 &&  y1 ==  y2){
        return true;
    }
    else{
        return false;
    }
}



int main() {
    
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
        refresh();        
        timeout(100);   
        snake.keybindings();
    snake.move();
    snake.move_segments();
    if(collision(snake.get_x(),food.get_x(),snake.get_y(),food.get_y())){
        food.random_xy();
        snake.add_segments();   
    }
}
    endwin();             
    return 0;
   }
