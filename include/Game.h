#ifndef _GAME_H_
#define _GAME_H_

namespace daybreak {

    class Game {
    public:
        virtual void init() = 0;
        virtual void update() = 0;
        virtual void render() = 0;
        virtual void end() = 0;
        virtual bool quit() = 0;
    };

}

#endif
