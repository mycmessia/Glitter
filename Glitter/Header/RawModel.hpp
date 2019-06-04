//
//  RawModel.hpp
//  GameEngine
//
//  Created by 梅宇宸 on 17/1/2.
//  Copyright © 2017年 梅宇宸. All rights reserved.
//

#ifndef RawModel_hpp
#define RawModel_hpp

class RawModel
{
public:
    RawModel (int id, int count);
    
    inline int GetVaoID () { return vaoID; }
    inline int GetVertexCount () { return vertexCount; }
    
private:
    int vaoID;
    int vertexCount;
};

#endif /* RawModel_hpp */
