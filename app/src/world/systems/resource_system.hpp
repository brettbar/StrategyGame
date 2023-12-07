#pragma once

namespace ResourceSystem {


  // struct Really {
  //   std::string resource;
  //   std::string value;

  //   template<class Archive>
  //   void serialize( Archive &ar ) {
  //     ar( CEREAL_NVP( resource ) );
  //     CEREAL_NVP( value );
  //   }
  // };
  // https://uscilab.github.io/cereal/serialization_archives.html
  // https://stackoverflow.com/questions/64865338/cereal-unable-to-deserialize-a-json-string-unhandled-exception
  // void epilogue( cereal::JSONInputArchive &, const Really & ) {}
  // void prologue( cereal::JSONInputArchive &, const Really & ) {}
  // inline void LoadData() {
  //   std::ifstream file( "data/really.json" );
  //   cereal::JSONInputArchive input{ file };

  //   Really resources;
  //   input( resources );

  //   std::cout << "SUCCESS INPUT" << std::endl;
  //   cereal::JSONOutputArchive pretty_print( std::cout );
  //   pretty_print( resources );
  // }

};// namespace ResourceSystem
