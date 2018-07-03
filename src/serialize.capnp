@0x86789622bf9371ab;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("Retro::Serialize");

using DataType = Text;

struct TypedSearchResult {
	address @0 :UInt64;
	mult @1 :UInt64;
	div @2 :UInt64;
	bias @3 :Int64;
	type @4 :DataType;
}

struct Search {
	currentResults @0 :List(TypedSearchResult);
	validTypes @1 : List(DataType);
}

struct Block {
	mem @0 :Data;
	offset @1 : UInt64;
}

struct SearchTuple {
	name @0 :Text;
	search @1 :Search;
	blocks @2 :List(Block);
}
