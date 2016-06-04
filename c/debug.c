#include <stdio.h>

#include "debug.h"
#include "value.h"

static int constantInstruction(Chunk* chunk, int i, const char* name) {
  uint8_t constant = chunk->code[i++];
  printf("%-16s %4d '", name, constant);
  printValue(chunk->constants.values[constant]);
  printf("'\n");
  return i;
}

int disassembleInstruction(Chunk* chunk, int i) {
  printf("%04d ", i);
  if (i > 0 && chunk->lines[i] == chunk->lines[i - 1]) {
    printf("  | ");
  } else {
    printf("%3d ", chunk->lines[i]);
  }
  
  uint8_t* code = chunk->code;
  uint8_t instruction = code[i++];
  
  switch (instruction) {
    case OP_CONSTANT:
      i = constantInstruction(chunk, i, "OP_CONSTANT");
      break;
      
    case OP_NIL: printf("OP_NIL\n"); break;
    case OP_TRUE: printf("OP_TRUE\n"); break;
    case OP_FALSE: printf("OP_FALSE\n"); break;
      
    case OP_POP: printf("OP_POP\n"); break;
      
    case OP_GET_LOCAL: {
      uint8_t slot = code[i++];
      printf("%-16s %4d\n", "OP_GET_LOCAL", slot);
      break;
    }
      
    case OP_SET_LOCAL: {
      uint8_t slot = code[i++];
      printf("%-16s %4d\n", "OP_SET_LOCAL", slot);
      break;
    }

    case OP_GET_GLOBAL:
      i = constantInstruction(chunk, i, "OP_GET_GLOBAL");
      break;
      
    case OP_DEFINE_GLOBAL:
      i = constantInstruction(chunk, i, "OP_DEFINE_GLOBAL");
      break;
      
    case OP_SET_GLOBAL:
      i = constantInstruction(chunk, i, "OP_SET_GLOBAL");
      break;
      
    case OP_GET_UPVALUE: {
      uint8_t slot = code[i++];
      printf("%-16s %4d\n", "OP_GET_UPVALUE", slot);
      break;
    }
      
    case OP_SET_UPVALUE: {
      uint8_t slot = code[i++];
      printf("%-16s %4d\n", "OP_SET_UPVALUE", slot);
      break;
    }
      
    case OP_GET_FIELD:
      i = constantInstruction(chunk, i, "OP_GET_FIELD");
      break;
      
    case OP_SET_FIELD:
      i = constantInstruction(chunk, i, "OP_SET_FIELD");
      break;
      
    case OP_GET_SUPER:
      i = constantInstruction(chunk, i, "OP_GET_SUPER");
      break;
      
    case OP_EQUAL: printf("OP_EQUAL\n"); break;
    case OP_GREATER: printf("OP_GREATER\n"); break;
    case OP_LESS: printf("OP_LESS\n"); break;
    case OP_ADD: printf("OP_ADD\n"); break;
    case OP_SUBTRACT: printf("OP_SUBTRACT\n"); break;
    case OP_MULTIPLY: printf("OP_MULTIPLY\n"); break;
    case OP_NOT: printf("OP_NOT\n"); break;
    case OP_DIVIDE: printf("OP_DIVIDE\n"); break;
    case OP_NEGATE: printf("OP_NEGATE\n"); break;
      
    case OP_JUMP: {
      uint16_t offset = (uint16_t)(code[i++] << 8);
      offset |= code[i++];
      printf("%-16s %4d -> %d\n", "OP_JUMP", offset, i + offset);
      break;
    }
      
    case OP_JUMP_IF_FALSE: {
      uint16_t offset = (uint16_t)(code[i++] << 8);
      offset |= code[i++];
      printf("%-16s %4d -> %d\n", "OP_JUMP_IF_FALSE", offset, i + offset);
      break;
    }
      
    case OP_LOOP: {
      uint16_t offset = (uint16_t)(code[i++] << 8);
      offset |= code[i++];
      printf("%-16s %4d -> %d\n", "OP_LOOP", offset, i - offset);
      break;
    }
      
    case OP_CALL_0:
    case OP_CALL_1:
    case OP_CALL_2:
    case OP_CALL_3:
    case OP_CALL_4:
    case OP_CALL_5:
    case OP_CALL_6:
    case OP_CALL_7:
    case OP_CALL_8:
      printf("OP_CALL_%d\n", instruction - OP_CALL_0);
      break;
      
    case OP_INVOKE_0: i = constantInstruction(chunk, i, "OP_INVOKE_0"); break;
    case OP_INVOKE_1: i = constantInstruction(chunk, i, "OP_INVOKE_1"); break;
    case OP_INVOKE_2: i = constantInstruction(chunk, i, "OP_INVOKE_2"); break;
    case OP_INVOKE_3: i = constantInstruction(chunk, i, "OP_INVOKE_3"); break;
    case OP_INVOKE_4: i = constantInstruction(chunk, i, "OP_INVOKE_4"); break;
    case OP_INVOKE_5: i = constantInstruction(chunk, i, "OP_INVOKE_5"); break;
    case OP_INVOKE_6: i = constantInstruction(chunk, i, "OP_INVOKE_6"); break;
    case OP_INVOKE_7: i = constantInstruction(chunk, i, "OP_INVOKE_7"); break;
    case OP_INVOKE_8: i = constantInstruction(chunk, i, "OP_INVOKE_8"); break;
      
    case OP_SUPER_0: i = constantInstruction(chunk, i, "OP_SUPER_0"); break;
    case OP_SUPER_1: i = constantInstruction(chunk, i, "OP_SUPER_1"); break;
    case OP_SUPER_2: i = constantInstruction(chunk, i, "OP_SUPER_2"); break;
    case OP_SUPER_3: i = constantInstruction(chunk, i, "OP_SUPER_3"); break;
    case OP_SUPER_4: i = constantInstruction(chunk, i, "OP_SUPER_4"); break;
    case OP_SUPER_5: i = constantInstruction(chunk, i, "OP_SUPER_5"); break;
    case OP_SUPER_6: i = constantInstruction(chunk, i, "OP_SUPER_6"); break;
    case OP_SUPER_7: i = constantInstruction(chunk, i, "OP_SUPER_7"); break;
    case OP_SUPER_8: i = constantInstruction(chunk, i, "OP_SUPER_8"); break;
      
    case OP_CLOSURE: {
      uint8_t constant = code[i++];
      printf("%-16s %4d ", "OP_CLOSURE", constant);
      printValue(chunk->constants.values[constant]);
      printf("\n");
      
      ObjFunction* closedFunction = AS_FUNCTION(chunk->constants.values[constant]);
      for (int j = 0; j < closedFunction->upvalueCount; j++) {
        int isLocal = code[i++];
        int index = code[i++];
        printf("%04d   |                     %s %d\n",
               i - 2, isLocal ? "local" : "upvalue", index);
      }
      break;
    }
      
    case OP_CLOSE_UPVALUE: printf("OP_CLOSE_UPVALUE\n"); break;
    case OP_RETURN: printf("OP_RETURN\n"); break;

    case OP_CLASS:
      i = constantInstruction(chunk, i, "OP_CLASS");
      break;
      
    case OP_SUBCLASS:
      i = constantInstruction(chunk, i, "OP_SUBCLASS");
      break;
  
    case OP_METHOD:
      i = constantInstruction(chunk, i, "OP_METHOD");
      break;
  }
  
  return i;
}

void disassembleChunk(Chunk* chunk, const char* name) {
  printf("-- %s --\n", name);

  for (int i = 0; i < chunk->count;) {
    i = disassembleInstruction(chunk, i);
  }
}
