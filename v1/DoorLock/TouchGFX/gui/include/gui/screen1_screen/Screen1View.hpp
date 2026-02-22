#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void click_one();
    virtual void click_two();
    virtual void click_three();
    virtual void click_four();
    virtual void click_five();
    virtual void click_six();
    virtual void click_seven();
    virtual void click_eight();
    virtual void click_nine();
    virtual void click_star();
    virtual void click_zero();
    virtual void click_sharp();
protected:
};

#endif // SCREEN1VIEW_HPP
