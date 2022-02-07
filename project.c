#include "spimcore.h"

/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
  //Daniela
  switch (ALUControl){
    case 0b000 :
      *ALUresult = A + B;
      break;

    case 0b001 :
      *ALUresult = A - B;
      break;

    case 0b010 :
      if(A<B)
        *ALUresult = 1;
      else
        *ALUresult = 0;
      break;

    case 0b011 :
      if(A<B)
        *ALUresult = 1;
      else
       *ALUresult = 0;
      break;

    case 0b100 :
      *ALUresult = A & B;
      break;

    case 0b101 :
      *ALUresult = A | B;
      break;

    case 0b110 :
      *ALUresult = B << 16;
      break;

    case 0b111 :
      *ALUresult = ~A;
      break;
      
    }

    if(ALUresult == 0)
      *Zero = (char) 1;
    else 
      *Zero = (char) 0;
}


/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
  //Daniela
  if (PC % 4 == 0) {
    *instruction = Mem[PC >> 2];
    return 0;
  }
  else 
    return 1;
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
  *op = instruction >> 26;
  *r1 = (instruction >> 21) & 0b11111;
  *r2 = (instruction >> 16) & 0b11111;
  *r3 = (instruction >> 11) & 0b11111;
  *funct = instruction & 0b111111;
  *offset = instruction & 0b1111111111111111;
  *jsec = instruction & 0b11111111111111111111111111;
}


/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
  switch (op) {
    // R-format
    case 0x00:
      controls -> RegDst = 0x1;
      controls -> Jump = 0x0;
      controls -> Branch = 0x0;
      controls -> MemRead = 0x0;
      controls -> MemtoReg = 0x0;
      controls -> ALUOp = 0x7;
      controls -> MemWrite = 0x0;
      controls -> ALUSrc = 0x0;
      controls -> RegWrite = 0x1;
      break;
    // jump
    case 0x02:
      controls -> RegDst = 0x2;
      controls -> Jump = 0x1;
      controls -> Branch = 0x0;
      controls -> MemRead = 0x0;
      controls -> MemtoReg = 0x0;
      controls -> ALUOp = 0x0;
      controls -> MemWrite = 0x0;
      controls -> ALUSrc = 0x0;
      controls -> RegWrite = 0x0;
      break;
    // branch eq
    case 0x04:
      controls -> RegDst = 0x2;
      controls -> Jump = 0x0;
      controls -> Branch = 0x1;
      controls -> MemRead = 0x0;
      controls -> MemtoReg = 0x2;
      controls -> ALUOp = 0x1;
      controls -> MemWrite = 0x0;
      controls -> ALUSrc = 0x0;
      controls -> RegWrite = 0x0;
      break;
    // add immediate
    case 0x08:
      controls -> RegDst = 0x0;
      controls -> Jump = 0x0;
      controls -> Branch = 0x0;
      controls -> MemRead = 0x0;
      controls -> MemtoReg = 0x0;
      controls -> ALUOp = 0x0;
      controls -> MemWrite = 0x0;
      controls -> ALUSrc = 0x1;
      controls -> RegWrite = 0x1;
      break;
    // set less than imm.
    case 0x0A:
      controls -> RegDst = 0x0;
      controls -> Jump = 0x0;
      controls -> Branch = 0x0;
      controls -> MemRead = 0x0;
      controls -> MemtoReg = 0x0;
      controls -> ALUOp = 0x2;
      controls -> MemWrite = 0x0;
      controls -> ALUSrc = 0x1;
      controls -> RegWrite = 0x1;
      break;
    // set less than imm. unsigned
    case 0x0B:
      controls -> RegDst = 0x0;
      controls -> Jump = 0x0;
      controls -> Branch = 0x0;
      controls -> MemRead = 0x0;
      controls -> MemtoReg = 0x0;
      controls -> ALUOp = 0x3;
      controls -> MemWrite = 0x0;
      controls -> ALUSrc = 0x1;
      controls -> RegWrite = 0x1;
      break;
    // load upper imm
    case 0x0F:
      controls -> RegDst = 0x0;
      controls -> Jump = 0x0;
      controls -> Branch = 0x0;
      controls -> MemRead = 0x0;
      controls -> MemtoReg = 0x0;
      controls -> ALUOp = 0x6;
      controls -> MemWrite = 0x0;
      controls -> ALUSrc = 0x1;
      controls -> RegWrite = 0x1;
      break;
    // load word
    case 0x23:
      controls -> RegDst = 0x0;
      controls -> Jump = 0x0;
      controls -> Branch = 0x0;
      controls -> MemRead = 0x1;
      controls -> MemtoReg = 0x1;
      controls -> ALUOp = 0x0;
      controls -> MemWrite = 0x0;
      controls -> ALUSrc = 0x1;
      controls -> RegWrite = 0x1;
      break;
    // store word
    case 0x2B:
      controls -> RegDst = 0x2;
      controls -> Jump = 0x0;
      controls -> Branch = 0x0;
      controls -> MemRead = 0x0;
      controls -> MemtoReg = 0x2;
      controls -> ALUOp = 0x0;
      controls -> MemWrite = 0x1;
      controls -> ALUSrc = 0x1;
      controls -> RegWrite = 0x0;
      break;
    default:
      return 1;
  }
  return 0;
}


/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
  *data1 = Reg[r1];
  *data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
  if ((offset >> 15) == 1) {
    *extended_value = (offset | 0xFFFF0000); 
  } else {
    *extended_value = (offset & 0x0000FFFF);
  }
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
  //Irene
  if(ALUSrc == 1) data2 = extended_value;
    if(ALUOp == 7)
    {
        switch (funct)
        {
            case 4:
                ALUOp = 6;break;            
            case 32:
                ALUOp = 0;break;
            case 34:
                ALUOp = 1;break; 
            case 36:
                ALUOp = 4;break;
            case 37:
                ALUOp = 5;break;
            case 39:
                ALUOp = 7;break;
            case 42:
                ALUOp = 2;break;
            case 43:
                ALUOp = 3;break;
            default:
                return 1;
        }
        ALU(data1, data2, ALUOp, ALUresult, Zero);
 
    } else ALU(data1, data2, ALUOp, ALUresult, Zero);

    return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
  //Irene
  if(MemWrite == 1)
  {
        if((ALUresult % 4) == 0) 
          Mem[ALUresult >> 2] = data2; 
        else
         return 1;
  }

    if(MemRead == 1)
    {
        if((ALUresult % 4) == 0)
          *memdata = Mem[ALUresult >> 2]; 
        else 
          return 1;
    }

    return 0;

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
  //Irene
  if((MemtoReg == 1) && (RegDst == 0) && (RegWrite == 1))
  {
    Reg[r2] = memdata;
  }
  else if((MemtoReg == 0) && (RegDst == 1) && (RegWrite == 1))
  {
    Reg[r3] = ALUresult;
  }
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{  
    *PC += 4;
    if(Jump == 1)
      *PC = (jsec << 2) | (*PC & 0xf0000000);
    if(Zero == 1 && Branch == 1)
      *PC += extended_value << 2;
}

