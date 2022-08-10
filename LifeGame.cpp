#include<SFML/Graphics.hpp>
#include<iostream>
#include<set>
#include<algorithm>
#include<fstream>
#include <boost/functional/hash.hpp>
#include <unordered_set>
#include <utility>

using namespace std;
using namespace sf;

const int res=10000;
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
            d1=view.getSize().x/wsiz.x;
            d2=view.getSize().y/wsiz.y;
            ifstream ifs("C:\\Users\\tomst\\Desktop\\LifeGame\\v.txt");
            while(!ifs.eof()){
                int a,b;
                ifs>>a>>b;
                alive.insert({a+500,b+500});
                world[a+500][b+500]=1;
            }
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
        std::unordered_set<pair<int,int>,boost::hash<pair<int,int>>> alive;
        //set<pair<int,int>> alive;
        set<pair<int,int>> flag;
        vector<pair<int,int>> add_;
        vector<pair<int,int>> minus_;
        vector<vector<int>> world;
        vector<vector<int>> tem_;
        set<pair<int,int>> tem;
        vector<pair<int,int>> vis;
        RectangleShape rec;
        const float squareGap=0.5;
        Vector2i mousepos=Mouse::getPosition(window);
        Vector2f pos;
        Vector2i mpos;
        Color c3; 
        float timer=0;
        float runtimer=0;
        void start();
        inline int cal(int i,int j);
        void updateWorld();
        void drawWorld(RenderWindow& window);
        void drawLines(RenderWindow& window);
        void drawWorld1(RenderWindow& window);
        void updateWorld1();
        void config();
        bool eventPoll(Event& e);
        void calNeighbor(int i,int j);
        vector<pair<int,int>> vp;
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
                timer=0;
                break;
            case Event::MouseButtonReleased:
                if(timer<0.28){
                    timer=0;
                    if(!sta){
                        pos=window.mapPixelToCoords(sf::Vector2i(e.mouseButton.x,e.mouseButton.y));
                        x_=pos.x/size_+mapEdge;
                        y_=pos.y/size_+mapEdge;
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
                    if(!sta){
                        ofstream out("C:\\Users\\tomst\\Desktop\\LifeGame\\v.txt");
                        for(auto it:alive){
                            out<<it.first-500<<" "<<it.second-500<<endl;
                        }
                        out.close();
                    }
                    sta=!sta;
                    return false;
                }
                break;
            case Event::MouseWheelScrolled:
                if(e.mouseWheelScroll.delta<0){
                    //cout<<-1<<" "<<e.mouseWheelScroll.delta<<endl;
                    for(int i=0;i<-e.mouseWheelScroll.delta;i++){
                        view.zoom(1.05);
                    }
                }else if(e.mouseWheelScroll.delta>0){
                    //cout<<1<<" "<<e.mouseWheelScroll.delta<<endl;
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
        runtimer+=clock.getElapsedTime().asSeconds();
        clock.restart();
        Event e;
        if(!eventPoll(e)){
            return;
        }
        
        if(timer>0.03){
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
    if(i<1||i>RN||j<1||j>RM){
        return 0;
    }
    int ret=0;
    for(int m=-1;m<=1;m++){
        for(int n=-1;n<=1;n++){
            if(m||n){
                if(world[i+m][j+n]==1){
                    ret++;
                }
            }
        }
    }
    return ret;
}
void LifeGame::calNeighbor(int i,int j){
    int c=cal(i,j);
    if(c>3||c<2){
        minus_.push_back({i,j});
    }
    int tem1,tem2;
    for(int m=-1;m<=1;m++){
        for(int n=-1;n<=1;n++){
            tem1=i+m;
            tem2=j+n;
            if(!world[tem1][tem2]&&(m||n)){
                world[tem1][tem2]=-1;
                vis.push_back({tem1,tem2});
                int c=cal(tem1,tem2);
                if(c==3){
                    add_.push_back({tem1,tem2});
                }
            }
        }
    }
}
void LifeGame::updateWorld(){
    if(alive.size()>100000){
        cout<<runtimer<<endl;
        cout<<alive.size()<<endl;
        exit(0);
    }
    int c=0;
    for(auto& it:alive){
        calNeighbor(it.first,it.second);
    }
    for(auto& it:vis){
        world[it.first][it.second]=0;
    }
    for(auto& it:add_){
        alive.insert(it);
        world[it.first][it.second]=1;
    }
    for(auto& it:minus_){
        alive.erase(it);
        world[it.first][it.second]=0;
    }
    vis.clear();
    add_.clear();
    minus_.clear();
}
void LifeGame::drawWorld(RenderWindow& window){
    float gap=(alive.size()*1.0/res)+1;
    int tem=gap;
    
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
}

int main(){
    LifeGame game;
    game.run();
    return 0;
}