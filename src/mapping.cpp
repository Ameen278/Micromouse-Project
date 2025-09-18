#include "mapping.h"

// مصفوفة المتاهة - كل خانة تحتوي على معلومات الجدران
byte maze[MAZE_SIZE][MAZE_SIZE];

// متغيرات للموقع الحالي
extern int currentX, currentY, currentDirection;

void initMaze() {
  // تهيئة المتاهة - كل الخانات غير مكتشفة
  for (int x = 0; x < MAZE_SIZE; x++) {
    for (int y = 0; y < MAZE_SIZE; y++) {
      maze[x][y] = 0;
    }
  }
  
  // إضافة الجدران الخارجية للمتاهة
  for (int i = 0; i < MAZE_SIZE; i++) {
    updateWall(i, 0, 0, true);           // جدار شمالي للصف الأول
    updateWall(i, MAZE_SIZE-1, 2, true); // جدار جنوبي للصف الأخير
    updateWall(0, i, 3, true);           // جدار غربي للعمود الأول
    updateWall(MAZE_SIZE-1, i, 1, true); // جدار شرقي للعمود الأخير
  }
  
  Serial.println("Maze initialized");
}

void updateWall(int x, int y, int direction, bool hasWall) {
  // التحقق من صحة الإحداثيات
  if (x < 0 || x >= MAZE_SIZE || y < 0 || y >= MAZE_SIZE) {
    return;
  }
  
  // تحديث الجدار في الاتجاه المحدد
  if (hasWall) {
    maze[x][y] |= (1 << direction);
  } else {
    maze[x][y] &= ~(1 << direction);
  }
  
  // تحديث الخانة المجاورة أيضاً
  int adjX = x, adjY = y;
  int adjDir = (direction + 2) % 4; // الاتجاه المقابل
  
  switch (direction) {
    case 0: adjY++; break; // شمال
    case 1: adjX++; break; // شرق
    case 2: adjY--; break; // جنوب
    case 3: adjX--; break; // غرب
  }
  
  // تحديث الخانة المجاورة إذا كانت داخل المتاهة
  if (adjX >= 0 && adjX < MAZE_SIZE && adjY >= 0 && adjY < MAZE_SIZE) {
    if (hasWall) {
      maze[adjX][adjY] |= (1 << adjDir);
    } else {
      maze[adjX][adjY] &= ~(1 << adjDir);
    }
  }
}

bool checkWall(int x, int y, int direction) {
  if (x < 0 || x >= MAZE_SIZE || y < 0 || y >= MAZE_SIZE) {
    return true; // خارج المتاهة = جدار
  }
  
  return (maze[x][y] & (1 << direction)) != 0;
}

void updateCurrentPosition() {
  // تحديث معلومات الجدران للموقع الحالي بناءً على المستشعرات
  updateWall(currentX, currentY, currentDirection, hasWallFront());
  updateWall(currentX, currentY, (currentDirection + 3) % 4, hasWallLeft());
  updateWall(currentX, currentY, (currentDirection + 1) % 4, hasWallRight());
}

void printMaze() {
  Serial.println("=== Current Maze Map ===");
  for (int y = MAZE_SIZE - 1; y >= 0; y--) {
    for (int x = 0; x < MAZE_SIZE; x++) {
      if (x == currentX && y == currentY) {
        Serial.print("R"); // موقع الروبوت
      } else if (maze[x][y] == 0) {
        Serial.print(".");  // غير مكتشف
      } else {
        Serial.print("#");  // مكتشف
      }
    }
    Serial.println();
  }
  Serial.println("========================");
}
