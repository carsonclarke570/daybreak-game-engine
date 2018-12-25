#include "include/Game.h"
#include "include/Daybreak.h"
#include "include/Scene.h"
#include "include/AssetManager.h"
#include "include/Timer.h"
#include "include/Manager.h"

using namespace daybreak;

struct MeshComponent : public Component {
    static const ComponentType m_type;

    Mesh* m_mesh;

    explicit MeshComponent(Mesh* mesh) : m_mesh(mesh) {
    }
};

class RenderSystem : public System {
public:
    explicit RenderSystem(Manager& manager) : System(manager) {
        ComponentTypeSet components;
        components.insert(MeshComponent::m_type);
        set_components(std::move(components));
    }

    void update(double_t delta, Entity entity) override {
        //MeshComponent& mesh = m_manager.get_store<MeshComponent>().get(entity);
    }

    void render(VkCommandBuffer cmd, Entity entity) override {
        //MeshComponent& mesh = m_manager.get_store<MeshComponent>().get(entity);
        //mesh.m_mesh->render(cmd);
    }
};

const ComponentType MeshComponent::m_type = 1;

class TestApplication : public Game {
public:
    Manager manager;
    AssetManager assets;

    Scene* m_test_scene;

    Mesh* mesh;

    double_t elapsed;

    struct UBO {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

    TestApplication() : mesh(nullptr), m_test_scene(nullptr) {
        elapsed = 0;
    }

    void init() override {
        assets.load_fragment_shader("frag", "../resources/shaders/frag.spv");
        assets.load_vertex_shader("vert", "../resources/shaders/vert.spv");
        assets.load_mesh("wolf", "../resources/models/Wolf.obj");

        std::vector<Shader*> shaders = {
                assets.get_shader("frag"),
                assets.get_shader("vert"),
        };

        std::vector<Binding> bindings = {
                {"transform", sizeof(UBO), 0, VK_SHADER_STAGE_VERTEX_BIT}
        };

        m_test_scene = new Scene(shaders, bindings);

        // Create component stores
        m_test_scene->create_store<MeshComponent>();

        // Add systems
        std::shared_ptr<System> sys(new RenderSystem(m_test_scene->get_manager()));
        m_test_scene->add_system(sys);

        // Create entities
        Entity dog = m_test_scene->create_entity();
        m_test_scene->add_component(dog, MeshComponent(assets.get_mesh("wolf")));
        m_test_scene->register_entity(dog);
    }

    void update(double_t delta) override {
        elapsed += delta;

        UBO ubo = {};
        ubo.model = glm::rotate(glm::mat4(1.0f), (float) (elapsed / 1000.0f) * glm::radians(90.0f),
                                glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f), API::viewport().width / API::viewport().height, 0.1f,
                                    10.0f);
        ubo.proj[1][1] *= -1;

        m_test_scene->update(delta);
        m_test_scene->update_uniform("transform", &ubo, sizeof(ubo));
    }

    void render() override {
        m_test_scene->render();
    }

    void end() override {
        assets.release_all_assets();
    }

    bool quit() override {
        return false;
    }
};

int main() {
    TestApplication app;
    Daybreak::load(&app);

    try {
        Daybreak::start();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}