#include <iostream>
#include <vector>
#include <ncurses.h> // getch(), printw(), refresh(), clear(), curs_set(), noecho(), keypad()
#include <unistd.h> 


enum Direction { UP, DOWN, LEFT, RIGHT };

void change_color(short color){
        start_color();
        use_default_colors(); //needed for init_pair method parameter:-1 
        init_pair(1,color, -1);// -1 for standart set background in terminal(unix based)
        attron(COLOR_PAIR(1)); 
}

void reset_color(){
    attroff(COLOR_PAIR(1));
}


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
            this->x  = rand() % 20;
            this->y  = rand() % 20;

        }

        void draw(){
            //change_color(COLOR_RED);
            mvprintw(this->y, this->x, "O0");
            //reset_color();
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
        mvprintw(this->y[i], this->x[i], "##");
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
    mvprintw(10,70,"Score: %d",score);
}


void grid(int width, int height,std::string choice){
    int start_value_x = 0;
    int last_value_x = 50;//width
    int start_value_y = 4;
    int last_value_y = 20; // height
    change_color(COLOR_MAGENTA);
    if(choice=="full"){
        for(int z = 0; z < width;z++){
            for(int i = 0; i < height; i++)
                if(z%1==0){
                    mvprintw(i,start_value_x+z,"|");
                }
            }
        }
    if(choice=="1"){
        mvprintw(start_value_y,start_value_x,"--------------------------------------------------");
        mvprintw(last_value_y,start_value_x,"---------------------------------------------------");
        for(int i = start_value_y; i < last_value_y; i++){
                mvprintw(i,start_value_x,"|");
            }
        for(int i = start_value_y; i < last_value_y; i++){
            mvprintw(i,last_value_x,"|");
            }
    reset_color();
}
}   




void game_loop(){
    int score = 0;    
    int width = 50;
    int height = 20;
    initscr();  
    curs_set(0);
    noecho();
    keypad(stdscr, TRUE); 
    Snake snake(11, 11);
    food food(20,20);
    snake.add_segments(); 
    snake.add_segments();
    while (true) {
        clear();    
        //grid(width,height,"1");   
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
            width = width -1;
            height = height -1;
            score++;
        }
    }
}




int main() {
    game_loop();
    return 0;
   }
