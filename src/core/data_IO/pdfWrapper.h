#pragma once 

#pragma comment

#include <string>
#include <PDFWriter.h>
#include <PDFPage.h>
#include <PageContentContext.h>
#include <PDFFormXObject.h>
#include <ResourcesDictionary.h>


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

public:
  int left_padding = 40;
  int top_padding = 842 - 40;
  int font_size = 20;



  void init(std::string fileLocation){
    pdfWriter.StartPDF(fileLocation,ePDFVersion13);
    page = new PDFPage();
    page->SetMediaBox(PDFRectangle(0,0,A4_Width,A4_Height));
    font = pdfWriter.GetFontForFile(standardFont);
    pageContentContext = pdfWriter.StartPageContentContext(page);

  }

  void end(){
    pdfWriter.EndPageContentContext(pageContentContext);
    pdfWriter.WritePageAndRelease(page);
    pdfWriter.EndPDF();
  }

  void textPiece_start(){
    pageContentContext->BT();
    pageContentContext->Tf(font,font_size);
    pageContentContext->k(fontStyle.r,fontStyle.g,fontStyle.b,fontStyle.a);
    pageContentContext->TL(font_size/4);
  }

  void addText(std::string text){
    pageContentContext->Tj(text);
  }

  void textPiece_end(){
    pageContentContext->ET();
  }

  void newLine(){
    pageContentContext->TStar();
  }

  void setFontStyle(int size, int r,  int g, int b,int a){
    pageContentContext->Tf(font,size);
    font_size = size;
    pageContentContext->k(r,g,b,a);
    fontStyle.r = r;
    fontStyle.g = g;
    fontStyle.b = b;
    fontStyle.a = a;
  }
  void setCursor(int x, int y){
    pageContentContext->Tm(font_size, 0,  0, font_size, x,  y);  
  }
};
