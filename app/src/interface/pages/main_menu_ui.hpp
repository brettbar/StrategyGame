#pragma once

#include "../../shared/common.hpp"

#include "../builders.hpp"
#include "../immediate.hpp"
#include <raylib.h>


namespace UI {


  inline void DrawMainMenu() {
    auto f = Iron::Forge();


    rect r = rect{ 0, 0, 250, 250 };
    auto grid = f->Grid( r, 3, 3 );

    f->TextLabel( grid->Slot( 0 ), "Actor", GREEN );
    bool okay_pressed = f->TextButton( grid->Slot( 2 ), "Okay", BLUE );

    if ( okay_pressed ) {
      printf( "okay pressed\n" );
    }

    f->Draw();
  }


  // inline sptr<Element> CreateMainMenuUI()
  // {
  //   return GridPanel( "ma
  //     .FixedSize(
  //     .Background
  //     .Children( {
  //       GridSlot(
  //         { 1, 1, 1, 1 },
  //         GridPanel( "ma
  //           .Background
  //           .Children( {
  //             GridSlot(
  //               { 0, 0, 0, 0 },
  //               TextButton( "main_men
  //
  //                 .Back
  //             ),
  //             GridSlot(
  //               { 0, 0, 1, 1 },
  //               TextButton( "main_men
  //
  //                 .Back
  //             ),
  //             GridSlot(
  //               { 0, 0, 2, 2 },
  //               TextButton( "main_men
  //
  //                 .Back
  //             ),
  //             GridSlot(
  //               { 0, 0, 3, 3 },
  //               TextButton( "main_men
  //
  //                 .Back
  //             ),
  //             GridSlot(
  //               { 0, 0, 4, 4 },
  //               TextButton( "main_men
  //
  //                 .Back
  //             ),
  //             GridSlot(
  //               { 0, 0, 5, 5 },
  //               TextButton( "main_men
  //
  //
  //
  //
  //       ),

  //     } );
  // }
};// namespace UI
