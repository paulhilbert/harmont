#ifndef _HARMONT_DEFERRED_RENDERER_HPP_
#define _HARMONT_DEFERRED_RENDERER_HPP_

#include "harmont.hpp"
#include "shadow_pass.hpp"
#include "renderable.hpp"

namespace harmont {


class deferred_renderer {
	public:
		typedef std::shared_ptr<deferred_renderer>        ptr_t;
		typedef std::weak_ptr<deferred_renderer>          wptr_t;
		typedef std::shared_ptr<const deferred_renderer>  const_ptr_t;
		typedef std::weak_ptr<const deferred_renderer>    const_wptr_t;
		typedef shadow_pass::geometry_callback_t          geometry_callback_t;
		typedef Eigen::AlignedBox<float, 3>               bounding_box_t;
        typedef std::map<std::string, renderable::ptr_t>  object_map_t;

		struct render_parameters_t {
			Eigen::Vector3f light_dir;
            Eigen::Vector3f background_color;
			float exposure;
			float shadow_bias;
            bool two_sided;
		};

		struct shadow_parameters_t {
			uint32_t resolution;
			uint32_t sample_count;
		};

	public:
		deferred_renderer(const render_parameters_t& render_parameters, const shadow_parameters_t& shadow_parameters, int width, int height);
		virtual ~deferred_renderer();

		void set_light_dir(const Eigen::Vector3f& light_dir);
		void set_background_color(const Eigen::Vector3f& color);

		float exposure() const;
		void set_exposure(float exposure);
		void delta_exposure(float delta);

		float shadow_bias() const;
		void set_shadow_bias(float bias);
		void delta_shadow_bias(float delta);

        bool two_sided() const;
        void set_two_sided(bool two_sided);
        void toggle_two_sided();

        float point_size() const;
        void set_point_size(float point_size);
        void delta_point_size(float delta);

		render_pass::ptr geometry_pass();
		render_pass::const_ptr geometry_pass() const;

        renderable::ptr_t object(std::string identifier);
        renderable::const_ptr_t object(std::string identifier) const;
        const object_map_t& objects() const;
        void add_object(std::string identifier, renderable::ptr_t object);
        void remove_object(std::string identifier);

        void dump_objects(const std::string& filepath) const;
        void reconstruct_objects(const std::string& filepath);

		void render(camera::ptr cam);
		void reshape(camera::ptr cam);

        void light_debug_add();
        void light_debug_rem();

        //void screenshot(camera::const_ptr cam, const std::string& filepath);

	protected:
        typedef enum {OPAQUE, TRANSPARENT, BOTH} geometry_visibility_t;

    protected:
        void   render_geometry_(shader_program::ptr program, pass_type_t type, geometry_visibility_t visibility);
		void   load_hdr_map_(std::string filename);
		static std::pair<float, float> get_near_far_(camera::const_ptr cam, const bounding_box_t& bbox);
		std::pair<bool, bool> object_predicates_() const;

	protected:
		render_pass_2d::ptr  clear_pass_;
		render_pass::ptr     geom_pass_;
		render_pass_2d::ptr  compose_pass_;
		render_pass_2d::ptr  compose_tex_pass_;
		render_pass::ptr     transp_geom_pass_;
		render_pass_2d::ptr  transp_compose_pass_;
		texture::ptr         depth_tex_;
		texture::ptr         gbuffer_tex_;
		texture::ptr         compose_tex_;
		texture::ptr         transp_accum_tex_;
		texture::ptr         transp_count_tex_;
		shadow_pass::ptr_t   shadow_pass_;
		Eigen::Vector3f      light_dir_;
		Eigen::Vector3f      background_color_;
		float                exposure_;
        float                shadow_bias_;
        bool                 two_sided_;
        float                point_size_;
        object_map_t         objects_;
        bbox_t               bbox_;

        // debug
        std::vector<Eigen::Vector3f> light_debug_;
};


} // harmont


#endif /* _HARMONT_DEFERRED_RENDERER_HPP_ */
