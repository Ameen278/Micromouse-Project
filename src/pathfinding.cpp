
#include "pathfinding.h"
#include "mapping.h"

// مصفوفة قيم الفيضان
int floodArray[MAZE_SIZE][MAZE_SIZE];

// إحداثيات الهدف (مركز المتاهة)
const int targetX1 = 7, targetY1 = 7;
const int targetX2 = 8, targetY2 = 7;
const int targetX3 = 7, targetY3 = 8;
const int targetX4 = 8, targetY4 = 8;

void initFloodFill() {
  // تهيئة جميع الخانات بقيمة عالية
  for (int x = 0; x < MAZE_SIZE; x++) {
    for (int y = 0; y < MAZE_SIZE; y++) {
      floodArray[x][y] = 255;
    }
  }
  
  // تعيين قيم الهدف (مركز المتاهة) إلى صفر
  floodArray[targetX1][targetY1] = 0;
  floodArray[targetX2][targetY2] = 0;
  floodArray[targetX3][targetY3] = 0;
  floodArray[targetX4][targetY4] = 0;
  
  Serial.println("Flood Fill initialized");
}

void floodFill(int startX, int startY) {
  // خوارزمية Flood Fill لحساب أقصر مسار
  bool changed = true;
  
  while (changed) {
    changed = false;
    
    for (int x = 0; x < MAZE_SIZE; x++) {
      for (int y = 0; y < MAZE_SIZE; y++) {
        if (floodArray[x][y] == 255) continue; // خانة غير مكتشفة
        
        // فحص الخانات المجاورة في الاتجاهات الأربعة
        for (int dir = 0; dir < 4; dir++) {
          int adjX = x, adjY = y;
          
          switch (dir) {
            case 0: adjY++; break; // شمال
            case 1: adjX++; break; // شرق
            case 2: adjY--; break; // جنوب
            case 3: adjX--; break; // غرب
          }
          
          // التحقق من الحدود وعدم وجود جدار
          if (adjX >= 0 && adjX < MAZE_SIZE && 
              adjY >= 0 && adjY < MAZE_SIZE && 
              !checkWall(x, y, dir)) {
            
            int newValue = floodArray[x][y] + 1;
            if (newValue < floodArray[adjX][adjY]) {
              floodArray[adjX][adjY] = newValue;
              changed = true;
            }
          }
        }
      }
    }
  }
  
  Serial.print("Flood Fill completed from (");
  Serial.print(startX);
  Serial.print(", ");
  Serial.print(startY);
  Serial.println(")");
}

int getNextDirection(int x, int y) {
  int minValue = floodArray[x][y];
  int bestDirection = currentDirection; // الاتجاه الحالي كافتراضي
  
  // فحص الاتجاهات الأربعة للعثور على أقل قيمة
  for (int dir = 0; dir < 4; dir++) {
    int adjX = x, adjY = y;
    
    switch (dir) {
      case 0: adjY++; break; // شمال
      case 1: adjX++; break; // شرق
      case 2: adjY--; break; // جنوب
      case 3: adjX--; break; // غرب
    }
    
    // التحقق من صحة الموقع وعدم وجود جدار
    if (adjX >= 0 && adjX < MAZE_SIZE && 
        adjY >= 0 && adjY < MAZE_SIZE && 
        !checkWall(x, y, dir) && 
        floodArray[adjX][adjY] < minValue) {
      
      minValue = floodArray[adjX][adjY];
      bestDirection = dir;
    }
  }
  
  return bestDirection;
}

bool isAtCenter(int x, int y) {
  return (x == targetX1 && y == targetY1) ||
         (x == targetX2 && y == targetY2) ||
         (x == targetX3 && y == targetY3) ||
         (x == targetX4 && y == targetY4);
}

void printFloodArray() {
  Serial.println("=== Flood Fill Values ===");
  for (int y = MAZE_SIZE - 1; y >= 0; y--) {
    for (int x = 0; x < MAZE_SIZE; x++) {
      if (floodArray[x][y] == 255) {
        Serial.print("  .");
      } else {
        Serial.print("  ");
        if (floodArray[x][y] < 10) Serial.print(" ");
        Serial.print(floodArray[x][y]);
      }
    }
    Serial.println();
  }
  Serial.println("=========================");
}

// دالة مساعدة لتحديث الموقع بناءً على الاتجاه
void updatePosition(int &x, int &y, int direction) {
  switch (direction) {
    case 0: y++; break; // شمال
    case 1: x++; break; // شرق
    case 2: y--; break; // جنوب
    case 3: x--; break; // غرب
  }
}