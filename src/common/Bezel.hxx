//============================================================================
//
//   SSSS    tt          lll  lll
//  SS  SS   tt           ll   ll
//  SS     tttttt  eeee   ll   ll   aaaa
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2024 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//============================================================================

#ifndef BEZEL_HXX
#define BEZEL_HXX

class OSystem;
class FBSurface;
class FrameBuffer;

#include "Rect.hxx"

/**
  This class handles the bezels.

  Bezels are loaded using a file name which is either a bezel name property or
  is autogenerated from the cart name property. The bezels can be any size and
  their transparent emulation window can be at any position. The position of
  the window can be determined automatically.

    +--------------------------------------+
    |                                      | display.h
    +--------------------------------------+
    |                                      |
    |      +---------------+               |
    |      |    window     |               |
    |      |               |               |
    |      | tia.h * zoom  |               |
    |      |               |               | bezel.h * zoom
    |      |               |               |
    |      +---------------+               |
    |                                      |
    +--------------------------------------+ size
    |                                      |
    +--------------------------------------+

  The bezel and window sizes and their ratios are used for correct scaling.

  @author  Thomas Jentzsch
*/

class Bezel
{
  public:
    explicit Bezel(OSystem& osystem);
    ~Bezel() = default;

    struct Info
    {
  private:
      bool _isShown{false};       // Is bezel shown?
      Common::Size _size{1, 1};   // Bezel size
      Common::Rect _window{1, 1}; // Area of transparent TIA window inside bezel

  public:
      explicit Info() = default;
      explicit Info(Common::Size size, Common::Rect window)
        : _isShown{true}, _size{size}, _window{window} { }

      bool isShown() const { return _isShown; }
      Common::Size size() const { return _size; }
      Common::Rect window() const { return _window; }

      // Ratios between bezel sizes and TIA window sizes
      double ratioW() const { return static_cast<double>(size().w) / window().w(); }
      double ratioH() const { return static_cast<double>(size().h) / window().h(); }
    };

    // Structure access methods
    const Info& info() const { return myInfo; }
    bool isShown() const { return myInfo.isShown(); }
    Common::Size size() const { return myInfo.size(); }
    Common::Rect window() const { return myInfo.window(); }
    // Ratio between bezel size and TIA window size
    double ratioW() const { return myInfo.ratioW(); }
    double ratioH() const { return myInfo.ratioH(); }

    /*
      Calculate size of a bezel border.
    */
    uInt32 borderSize(uInt32 x, uInt32 y, uInt32 size, Int32 step) const;

    /*
      Load the bezel.
    */
    bool load();

    /*
      Display scaled bezel.
    */
    void apply();

    /*
      Render bezel surface
    */
    void render();

  private:
    /*
      Generate bezel file name.
    */
    string getName(int& index) const;

  private:
    // The parent system for the bezel
    OSystem& myOSystem;

    // Pointer to the FrameBuffer object
    FrameBuffer& myFB;

    // The bezel surface which blends over the TIA surface
    shared_ptr<FBSurface> mySurface;

    // Bezel info structure
    Info myInfo;

  private:
    // Following constructors and assignment operators not supported
    Bezel() = delete;
    Bezel(const Bezel&) = delete;
    Bezel(Bezel&&) = delete;
    Bezel& operator=(const Bezel&) = delete;
    Bezel& operator=(Bezel&&) = delete;
};

#endif
