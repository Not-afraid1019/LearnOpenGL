// Not_afraid
// 2024/11/21

#ifndef LEARNOPENGL_CHECKERROR_H
#define LEARNOPENGL_CHECKERROR_H

#ifdef DEBUG
#define GL_CALL(func) func; checkError()
#else
#define GL_CALL(func) func
#endif

void checkError();

#endif //LEARNOPENGL_CHECKERROR_H
