#pragma once 

#pragma comment

#include <string>
#include <PDFWriter.h>
#include <PDFPage.h>
#include <PageContentContext.h>
#include <PDFFormXObject.h>
#include <ResourcesDictionary.h>
#include <PDFUsedFont.h>


/// THIS FILE WAS PROVIDED BY OMAR_MAGDY_GHAZY EPE2024 As an abstraction for PDFHummus/PDFWriter lib no warranty is provided



/*!
  @file   pdfWrapper.h
  @brief  This file basically abstracts the features of 
          PDFWriter/PDFHummus library for ease of use across different parts of project
*/



#define A4_Width  595
#define A4_Height 842

#define LEFT(n)(n)
#define TOP(n)(A4_Height - n)
#define standardFont "resources/OpenSans-Italic.ttf"

class pdf_wrap{
  PDFWriter pdfWriter;
  PDFPage* page;
  PageContentContext* pageContentContext;
  PDFUsedFont* font;

  struct {
    int r = 0;
    int g = 0;
    int b = 0;
    int a = 1;
  }fontStyle;

  unsigned int pos_x = 0;
  unsigned int pos_y = 0;


public:
  int left_padding = 30;
  int top_padding = 30;
  float font_size = 5;
  int interline_gap = font_size*4;


  void init(std::string fileLocation){
    pdfWriter.StartPDF(fileLocation,ePDFVersion13);
    font = pdfWriter.GetFontForFile(standardFont);
  }

  void end(){
    pdfWriter.EndPDF();
  }

  void newPage(){
    page = new PDFPage();
    page->SetMediaBox(PDFRectangle(0,0,A4_Width,A4_Height));
    pageContentContext = pdfWriter.StartPageContentContext(page);
    pos_x = 0;
    pos_y = 0;
  }

  void endPage(){
    pdfWriter.EndPageContentContext(pageContentContext);
    pdfWriter.WritePageAndRelease(page);
  }

  void textPiece_start(){
    pageContentContext->BT();
    pageContentContext->Tf(font,font_size);
    pageContentContext->k(fontStyle.r,fontStyle.g,fontStyle.b,fontStyle.a);
  }

  void addText(std::string text){
    pageContentContext->Tj(text);
    PDFUsedFont::TextMeasures textDimensions = font->CalculateTextDimensions(text,font_size);
    pos_x += textDimensions.xMax;
  }

  void textPiece_end(){
    pageContentContext->ET();
  }

  void newLine(){
    pos_x = LEFT(left_padding);
    pos_y -= interline_gap;
    pageContentContext->Tm(1,0,0,1,LEFT(left_padding),  pos_y);
  }

  void setFontStyle(float size, int r,  int g, int b,int a){
    pageContentContext->Tf(font,size);
    font_size = size;
    pageContentContext->k(r,g,b,a);
    fontStyle.r = r;
    fontStyle.g = g;
    fontStyle.b = b;
    fontStyle.a = a;
    pageContentContext->TL(font_size);
  }

  void setCursor(int x, int y){
    pageContentContext->Tm(1, 0,  0, 1, x,  y);  
    pos_x = x;
    pos_y = y;
  }


  int get_posX(){
    return pos_x;
  }

  int get_posY(){
    return pos_y;
  }

  void center_text(std::string word){
    PDFUsedFont::TextMeasures textDimensions = font->CalculateTextDimensions(word,font_size); 
    pageContentContext->Td(A4_Width/2 - textDimensions.width/2 - left_padding, 0);
    addText(word);
    pageContentContext->Td(-(A4_Width/2 - textDimensions.width/2 - left_padding), 0);
  }
};

