#pragma once


#include "aura/graphics/image/context.h"


namespace imaging_wic
{


   class CLASS_DECL_IMAGING_WIC image_context :
      virtual public ::image::image_context
   {
   public:


      //__creatable_from_base(image_context, ::image_context);


      ::pointer<handler_manager>       m_pmanagerImageLoadFastQueue; // probably low latency / fast
      ::pointer<handler_manager>       m_pmanagerImageLoadSlowQueue; // possibly high latency / slow

      image_context();
      ~image_context() override;


      void initialize(::particle * pparticle) override;

      void destroy() override;


      //virtual void _load_image(::image_context * pimagecontext, ::image::image * pimageParam, const ::payload & payloadFile, bool bSync, bool bCreateHelperMaps) override;
      void _load_image(::image::image* pimage, const ::payload& payloadFile, const ::image::load_options & loadoptions = ::image::load_options()) override;

      void _load_image(::image::image * pimage, ::pointer<::image::image_frame_array>& pframea, ::memory & memory) override;
      void save_image(memory & memory, ::image::image * pimage, const ::image::encoding_options & encodingoptions) override;


      //void _load_icon(::image::icon * picon, const ::payload & payloadFile);


      //virtual void _load_image(::image::image *  pimage, const ::payload& payloadFile, bool bSync, bool bCreateHelperMaps);


      //virtual void _load_image(::image::image *  pimage, ::pointer<image_frame_array> pframea, ::memory_pointer pmemory);
      //virtual void save_image(memory& memory, const ::image* pimage, ::save_image* psaveimage);


      //virtual void load_cursor(::draw2d::cursor* pcursor, ::file::path path, bool bSync, bool bCache = true) override;

      //virtual void load_svg(::image::image *  pimage, ::memory_pointer pmemory);


#ifdef UNIVERSAL_WINDOWS
      virtual bool _desk_to_image(::image::image *  pimage);
      virtual bool _desk_has_image();
      virtual bool _image_to_desk(::image::image *  pimage);
#endif


      virtual bool _save_image(::file::file* pfile, ::image::image* pimage, const ::image::encoding_options & encodingoptions);

      void _os_load_image(::image::image * pimage, memory & memory) override;


   };


} // namespace imaging_wic



