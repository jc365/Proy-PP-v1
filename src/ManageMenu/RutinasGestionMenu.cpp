  //--- variables generales para la gestion del menu
  short int m ;
  short int c ;
  short int lineas_LCD = 3 ;

  //--- prototipado de funciones
  void escribe_linea_menu(short int, short int) ;
  void escribe_cursor(short int) ;
  short int ajusta(short int ) ;
  void m_boton_intro(short int , short int ) ;



  void menu_ini() {
    m = 1 ;
    c = 1 ;
    // escribe el menu sobre la totalidad de lineas disponibles en el LCD
    lcd.clear() ;
    for (short int i = 1; i < lineas_LCD; i++) {
      escribe_linea_menu(i, m);
    }
    escribe_cursor(c);
  }

  void m_boton_mas() {
    if (c == lineas_LCD) {
      //- mantiene el cursor en la misma linea y reescribe desde el siguiente menu
      m++ ;
      for (short int i = 1; i < lineas_LCD; i++) {
        escribe_linea_menu(i, m);
      }
    }
    else {
      //- avanza el cursor una linea
      c++ ;
    }
    escribe_cursor(c);
  }

  void m_boton_menos() {
    if (c == 1) {
      //-- mantiene el cursor en la misma linea y reescribe desde el anterior menu
      m-- ;
      for (short int i = 1; i < lineas_LCD; i++) {
        escribe_linea_menu(i, m);
      }
    }
    else {
      c-- ;
    }
    escribe_cursor(c);
  }

  void m_boton_intro(short int m, short int c) {
    //- mostar submenu del menu seleccionado
    //    var<m>: menu cabecera de la pantalla
    //    var<c>: linea en la que esta el cursor
  }



  void escribe_linea_menu(short int i, short int m) {
    if (i == 1) {
      lcd.clear() ;
    }
    lcd.print( M[ajusta(i+m-1) ];
  }

  void escribe_cursor(short int x) {
    short int pos = 5;  // posicion de la linea donde pone el cursor
    lcd.setCursor(pos, x) ;
  }

  short int ajusta(short int x) {
    short int r = (x > lineas_LCD) ? x % lineas_LCD : x ;
    return r;
  }
