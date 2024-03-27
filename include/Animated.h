#ifndef ANIMATED_H
#define ANIMATED_H
#define ANIMATED_DECLARATION virtual bool animateOn(Animation* const animation, const int type, const int selector, const float value){return animation->addFrameFunction(type, this, selector, value);}
#endif // ANIMATED_H
