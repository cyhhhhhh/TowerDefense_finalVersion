#ifndef TARGET_H
#define TARGET_H

class Target
{
public:
    virtual void getDamage(int damage,bool ice);
    virtual bool check(Target* target);
};

#endif // TARGET_H
