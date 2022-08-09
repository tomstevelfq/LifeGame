#include<SFML/Graphics.hpp>
#include<iostream>
#include<set>
#include<algorithm>
using namespace std;
using namespace sf;

class LifeGame{
    public:
        LifeGame(){
            window.create(VideoMode(80*size_,50*size_),"LifeGame");
            wsiz=window.getSize();
            view.setSize(wsiz.x,wsiz.y);
            view.setCenter(wsiz.x/2,wsiz.y/2);
            //view.zoom(0.5);
            vsiz=view.getSize();
            c1=Color(237,87,54);
            c2=Color(66,80,102);
            c3=Color(30,132,73);
            world=vector<vector<int>>(N,vector<int>(M,0));
            tem=world;
            d1=view.getSize().x/wsiz.x;
            d2=view.getSize().y/wsiz.y;
        }
        void run();

    private:
        RenderWindow window;
        View view;
        const int mapEdge=3;//边缘宽度
        Color c1;
        Color c2;
        bool sta=false;
        Vector2u wsiz;
        Vector2f vsiz;
        float d1;
        float d2;
        const int RM=2000;//格子数量
        const int RN=2000;//格子数量
        const int size_=25;//格子大小
        const int M=RM+mapEdge*2;
        const int N=RN+mapEdge*2;
        set<pair<int,int>> alive;
        set<pair<int,int>> flag;
        vector<vector<int>> world;
        vector<vector<int>> tem;
        RectangleShape rec;
        const float squareGap=0.5;
        Vector2i mousepos=Mouse::getPosition(window);
        Vector2f pos;
        Vector2i mpos;
        Color c3; 
        float timer=0;
        void start();
        int cal(int i,int j);
        void updateWorld();
        void drawWorld(RenderWindow& window);
        void drawLines(RenderWindow& window);
        void config();
        bool eventPoll(Event& e);
        void calNeighbor(int i,int j);
};
void LifeGame::run(){
    while(window.isOpen()){
        Event e;
        while(window.pollEvent(e)){
            switch(e.type){
                case Event::Closed:
                    window.close();
                    break;
            }
            config();
            start();
        }
                    
    }
}
bool LifeGame::eventPoll(Event& e){
    int x_;
    int y_;
     while(window.pollEvent(e)){
        switch(e.type){
            case Event::Closed:
                window.close();
                break;
            case Event::MouseButtonPressed:
                // if(!sta){
                //     pos=window.mapPixelToCoords(sf::Vector2i(e.mouseButton.x,e.mouseButton.y));
                //     x_=pos.x/size_+mapEdge;
                //     y_=pos.y/size_+mapEdge;
                //     //cout<<pos.x<<" "<<pos.y<<endl;
                //     //cout<<x_<<"  "<<y_<<endl;
                //     world[y_][x_]=!world[y_][x_]; 
                // }
                timer=0;
                break;
            case Event::MouseButtonReleased:
                if(timer<0.4){
                    timer=0;
                    if(!sta){
                        pos=window.mapPixelToCoords(sf::Vector2i(e.mouseButton.x,e.mouseButton.y));
                        x_=pos.x/size_+mapEdge;
                        y_=pos.y/size_+mapEdge;
                        //cout<<pos.x<<" "<<pos.y<<endl;
                        //cout<<x_<<"  "<<y_<<endl;
                        if(world[y_][x_]){
                            world[y_][x_]=!world[y_][x_];
                            alive.erase({y_,x_});
                        }else{
                            world[y_][x_]=!world[y_][x_];
                            alive.insert({y_,x_});
                        }
                    }
                }
                break;
            case Event::KeyPressed:
                if(e.key.code==Keyboard::Q){
                    sta=!sta;
                    return false;
                }
                break;
            case Event::MouseWheelScrolled:
                if(e.mouseWheelScroll.delta<0){
                    cout<<-1<<" "<<e.mouseWheelScroll.delta<<endl;
                    for(int i=0;i<-e.mouseWheelScroll.delta;i++){
                        view.zoom(1.05);
                    }
                }else if(e.mouseWheelScroll.delta>0){
                    cout<<1<<" "<<e.mouseWheelScroll.delta<<endl;
                    for(int i=0;i<e.mouseWheelScroll.delta;i++){
                        view.zoom(0.95);
                    }
                    
                }
                d1=view.getSize().x/wsiz.x;
                d2=view.getSize().y/wsiz.y;
                break; 
            case Event::MouseMoved:
                if(Mouse::isButtonPressed(Mouse::Left)){
                    auto tem=Mouse::getPosition(window);
                    view.move(d1*(mousepos.x-tem.x),d2*(mousepos.y-tem.y));
                    mousepos=tem;
                }else{
                    mousepos=Mouse::getPosition(window);
                }
                break;
        }
    }
    return true;
}
void LifeGame::config(){
    Clock clock;
    while(window.isOpen()){
        timer+=clock.getElapsedTime().asSeconds();
        clock.restart();
        Event e;
        if(!eventPoll(e)){
            return;
        }
        window.clear(c1);
        drawLines(window);
        drawWorld(window);
        window.setView(view);
        window.display();
    }
}
void LifeGame::start(){
    float timer=0;
    Clock clock;
    while(window.isOpen()){
        timer+=clock.getElapsedTime().asSeconds();
        clock.restart();
        Event e;
        if(!eventPoll(e)){
            return;
        }
        if(timer>0.02){
            timer=0;
            window.clear(c1);
            drawLines(window);
            drawWorld(window);
            window.setView(view);
            window.display();
            updateWorld();
        }
    }
}
int LifeGame::cal(int i,int j){
    int ret=0;
    for(int m=-1;m<=1;m++){
        for(int n=-1;n<=1;n++){
            if(m||n){
                if(tem[i+m][j+n]){
                    ret++;
                }
            }
        }
    }
    return ret;
}
void LifeGame::calNeighbor(int i,int j){
    int c=cal(i,j);
    if(c==3){
        world[i][j]=1;
        alive.insert({i,j});
    }else if(c==2&&world[i][j]){
        world[i][j]=1;
        alive.insert({i,j});
    }else{
        world[i][j]=0;
        alive.erase({i,j});
    }
    int tem1,tem2;
    for(int m=-1;m<=1;m++){
        for(int n=-1;n<=1;n++){
            if(m||n){
                tem1=i+m;
                tem2=j+n;
                if(flag.find({tem1,tem2})==flag.end()){
                    flag.insert({tem1,tem2});
                    int c=cal(tem1,tem2);
                    if(c==3){
                        world[tem1][tem2]=1;
                        alive.insert({tem1,tem2});
                    }else if(c==2&&world[tem1][tem2]){
                        world[tem1][tem2]=1;
                        alive.insert({tem1,tem2});
                    }else{
                        world[tem1][tem2]=0;
                        alive.erase({tem1,tem2});
                    }
                }
            }
        }
    }
}
void LifeGame::updateWorld(){
    tem=world;
    int c=0;
    // for(int i=1;i<N-1;i++){
    //     for(int j=1;j<M-1;j++){
    //         c=cal(i,j);
    //         if(c==3){
    //             world[i][j]=1;
    //         }else if(c==2&&world[i][j]){
    //             world[i][j]=1;
    //         }else{
    //             world[i][j]=0;
    //         }
    //     }
    // }
    flag.clear();
    auto tem=alive;
    for(auto& it:tem){
        flag.insert(it);
        calNeighbor(it.first,it.second);
    }
}
void LifeGame::drawWorld(RenderWindow& window){
    // for(int i=0;i<RM;i++){
    //     for(int j=0;j<RN;j++){
    //         if(world[j+mapEdge][i+mapEdge]){
    //             rec.setFillColor(c3);
    //             rec.setSize({size_-2*squareGap,size_-2*squareGap});
    //             rec.setPosition(i*size_+squareGap,j*size_+squareGap);
    //             window.draw(rec);
    //         }
    //     }
    // }
    for(auto it:alive){
        if(it.first>=mapEdge&&it.second>=mapEdge){
            rec.setFillColor(c3);
            rec.setSize({size_-2*squareGap,size_-2*squareGap});
            rec.setPosition((it.second-mapEdge)*size_+squareGap,(it.first-mapEdge)*size_+squareGap);
            window.draw(rec);
        }
    }
}
void LifeGame::drawLines(RenderWindow& window){
    VertexArray v(LineStrip,2);
    v[0].color=c2;
    v[1].color=c2;
    for(int i=0;i<RN;i++){
        v[0].position=Vector2f(0,i*size_);
        v[1].position=Vector2f(RM*size_,i*size_);
        window.draw(v);
    }
    for(int i=0;i<RM;i++){
        v[0].position=Vector2f(i*size_,0);
        v[1].position=Vector2f(i*size_,RN*size_);
        window.draw(v);
    }
    // for(int i=0;i<RM;i++){
    //     for(int j=0;j<RN;j++){
    //         v[0].position=Vector2f(i*size_,0);
    //         v[1].position=Vector2f(i*size_,RN*size_);
    //         window.draw(v);
    //         v[0].position=Vector2f(0,i*size_);
    //         v[1].position=Vector2f(RM*size_,i*size_);
    //         window.draw(v);
    //     }
    // }
}

int main(){
    LifeGame game;
    game.run();
    return 0;
}