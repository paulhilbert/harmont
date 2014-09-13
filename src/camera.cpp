#include <camera.hpp>

#include <view.hpp>

namespace harmont {

camera::camera(camera_model::ptr model, int width, int height, float fov, float near, float far, bool ortho) : model_(model), width_(width), height_(height), fov_(fov), near_(near), far_(far), ortho_(ortho) {
    reshape(width_, height_);
}

camera::~camera() {
}

camera_model::ptr camera::model() {
    return model_;
}

camera_model::const_ptr camera::model() const {
    return model_;
}

void camera::set_model(camera_model::ptr model) {
    model->set_up(model_->up());
    model->set_position(model_->position());
    model->set_look_at(model_->look_at());
    model_ = model;
}

camera::vec3_t camera::position() const {
    return model_->position();
}

camera::vec3_t camera::look_at() const {
    return model_->look_at();
}

camera::vec3_t camera::forward() const {
    return model_->forward();
}

camera::vec3_t camera::up() const {
    return model_->up();
}

camera::vec3_t camera::right() const {
    return model_->right();
}

int camera::width() const {
    return width_;
}

int camera::height() const {
    return height_;
}

float camera::near() const {
    return near_;
}

float camera::far() const {
    return far_;
}

float camera::fov() const {
    return fov_;
}

const camera::mat4_t& camera::view_matrix() const {
    return model_->view_matrix();
}

const camera::mat3_t& camera::view_normal_matrix() const {
    return model_->view_normal_matrix();
}

const camera::mat4_t& camera::projection_matrix() const {
    return projection_;
}

void camera::set_position(const vec3_t& position) {
    model_->set_position(position);
}

void camera::set_look_at(const vec3_t& look_at) {
    model_->set_look_at(look_at);
}

void camera::set_forward(const vec3_t& forward) {
    model_->set_forward(forward);
}

void camera::set_up(const vec3_t& up) {
    model_->set_up(up);
}

void camera::set_right(const vec3_t& right) {
    model_->set_right(right);
}

void camera::set_near(float near) {
    near_ = near;
    reshape(width_, height_);
}

void camera::set_far(float far) {
    far_ = far;
    reshape(width_, height_);
}

void camera::set_fov(float fov) {
    fov_ = fov;
    reshape(width_, height_);
}

bool camera::ortho() const {
    return ortho_;
}

void camera::set_ortho(bool state) {
    ortho_ = state;
    reshape(width_, height_);
}

void camera::toggle_ortho() {
    ortho_ = !ortho_;
    reshape(width_, height_);
}

camera::ray_t camera::pick_ray(int x, int y) const {
    mat4_t view_mat = model_->view_matrix();
    Eigen::Matrix<int,4,1> viewport(0, 0, width_, height_);
    vec3_t origin = unproject(vec3_t(static_cast<float>(x), static_cast<float>(y), near_), view_mat, projection_, viewport);
    vec3_t direction = unproject(vec3_t(static_cast<float>(x), static_cast<float>(y), far_), view_mat, projection_, viewport) - origin;
    direction.normalize();
    return {origin, direction};
}

void camera::reshape(int width, int height) {
    float aspect = std::max(width, height) / std::min(width, height);
	projection_ = perspective(fov_, aspect, near_, far_);
    mat4_t view_mat = model_->view_matrix();
	if (ortho_) {
		Eigen::Vector4f look_at;
		look_at << model_->look_at(), 1.f;
		look_at = view_mat * look_at;
		Eigen::Vector4f right = look_at;
		right[0] += 1.f;

		float factor = static_cast<float>(tan(fov_ * M_PI / 180.f) * 20.0);
		mat4_t ortho_mat = harmont::ortho(-aspect*factor, aspect*factor, -factor, factor, near_, far_);
		Eigen::Vector4f p0 = projection_*look_at, p1 = projection_*right, o0 = ortho_mat*look_at, o1 = ortho_mat*right;
		p0.head(3) /= p0[3]; p1.head(3) /= p1[3];
		float scale = (p1.head(3)-p0.head(3)).norm() / (o1.head(3)-o0.head(3)).norm();
		mat4_t scale_mat = mat4_t::Identity();
		scale_mat.block<3,3>(0,0) *= scale;
		projection_ = ortho_mat * scale_mat;
	}
}

void camera::update(vec3_t translational, vec3_t rotational) {
    model_->update(translational, rotational, ortho_);
}

} // harmont