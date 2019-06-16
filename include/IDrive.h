#ifndef IDRIVE_H
#define IDRIVE_H



class IDrive {
public:
    virtual ~IDrive() { }
    
    virtual void execute() = 0;
};

#endif /* IDrive_h */
