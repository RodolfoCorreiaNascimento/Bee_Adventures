/*
 * Game: Bee's Adventure
 * Author: Rodolfo Nascimento
 * Contact: espanharodofera@gmail.com
 */

#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_size.h"
#include "bn_blending.h"
#include "bn_bgs_mosaic.h"
#include "bn_bg_palettes.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_actions.h"
#include "bn_regular_bg_builder.h"
#include "bn_regular_bg_attributes.h"
#include "bn_regular_bg_position_hbe_ptr.h"
#include "bn_regular_bg_attributes_hbe_ptr.h"
#include "bn_sprites_mosaic.h"
#include "bn_sprite_actions.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_first_attributes.h"
#include "bn_sprite_third_attributes.h"
#include "bn_sprite_position_hbe_ptr.h"
#include "bn_sprite_first_attributes_hbe_ptr.h"
#include "bn_sprite_third_attributes_hbe_ptr.h"
#include "bn_sprite_affine_second_attributes.h"
#include "bn_sprite_regular_second_attributes.h"
#include "bn_sprite_affine_second_attributes_hbe_ptr.h"
#include "bn_sprite_regular_second_attributes_hbe_ptr.h"
#include "bn_affine_bg_pa_register_hbe_ptr.h"
#include "bn_affine_bg_pd_register_hbe_ptr.h"
#include "bn_affine_bg_dx_register_hbe_ptr.h"
#include "bn_affine_bg_dy_register_hbe_ptr.h"
#include <vector>
#include "bn_regular_bg_ptr.h"

// Sprites Headers Files
#include "bn_sprite_items_bee.h"
#include "bn_sprite_items_bee_shoot.h"
#include "bn_regular_bg_items_red.h"
#include "bn_affine_bg_items_clouds.h"

#define MAX_BULLETS 20 // Número máximo de balas
#define BULLET_SPEED 2 // Velocidade das balas

int num_bullets = 0; // Variável para controlar o número de balas

int main()
{
    bn::core::init();

    // BG sprite
    bn::regular_bg_ptr red_bg = bn::regular_bg_items::red.create_bg(0, 0);
    red_bg.set_priority(2);

    // Clouds sprite
    bn::affine_bg_ptr clouds_bg = bn::affine_bg_items::clouds.create_bg(0, 0);
    clouds_bg.set_priority(1);
    clouds_bg.set_blending_enabled(true);
    bn::blending::set_transparency_alpha(0.1);

    // Player sprite
    bn::sprite_ptr bee = bn::sprite_items::bee.create_sprite(0, 0);
    bee.set_bg_priority(0);

    // Crie um vetor de bn::sprite_ptr
    std::vector<bn::sprite_ptr> bullets;

    // Reserve espaço para o vetor (opcional, mas pode melhorar o desempenho)
    bullets.reserve(MAX_BULLETS);

    while (true)
    {
        bn::core::update();

        // Atualize a posição X e Y do background com base na posição do jogador
        red_bg.set_y(red_bg.y() + 1);

        // Se segurar o direcional 'right' executar uma função
        if (bn::keypad::right_held())
        {
            // Atualize a posição X do sprite para movê-lo para a direita
            bee.set_x(bee.x() + 1);

            // Verifique se o sprite colidiu com a borda direita da tela do GBA
            if (bee.x() > ((bn::display::width() / 2) - bee.dimensions().width()))
            {
                // Ajuste a posição X do sprite para que ele fique dentro da tela
                bee.set_x((bn::display::width() / 2) - bee.dimensions().width());
            }
        }

        // Se segurar o direcional 'left' executar uma função
        if (bn::keypad::left_held())
        {
            // Atualize a posição X do sprite para movê-lo para a esquerda
            bee.set_x(bee.x() - 1);

            // Verifique se o sprite colidiu com a borda esquerda da tela do GBA
            if (bee.x() < -104)
            {
                // Ajuste a posição X do sprite para que ele fique dentro da tela
                bee.set_x(-104); // já descontando o width da bee
            }
        }

        // Se segurar o direcional 'up' executar uma função
        if (bn::keypad::up_held())
        {
            // Atualize a posição y do sprite para movê-lo para cima
            bee.set_y(bee.y() - 1);

            // Verifique se o sprite colidiu com a borda superior da tela do GBA
            if (bee.y() < -64)
            {
                // Ajuste a posição y do sprite para que ele fique dentro da tela
                bee.set_y(-64);
            }
        }

        // Se segurar o direcional 'down' executar uma função
        if (bn::keypad::down_held())
        {
            // Atualize a posição y do sprite para movê-lo para baixo
            bee.set_y(bee.y() + 1);

            // Verifique se o sprite colidiu com a borda inferior da tela do GBA
            if (bee.y() > ((bn::display::height() / 2) - bee.dimensions().height()))
            {
                // Ajuste a posição y do sprite para que ele fique dentro da tela
                bee.set_y((bn::display::height() / 2) - bee.dimensions().height());
            }
        }

        // Se o botão A for pressionado e o número atual de balas for menor que o máximo permitido
        if (bn::keypad::a_pressed() && num_bullets < MAX_BULLETS)
        {

            // Crie uma nova bala no array de balas
            bullets.push_back(bn::sprite_items::bee_shoot.create_sprite(bee.x(),
                                                                        bee.y()));
          
            num_bullets++; // Incremente o número de balas
        }

        // Atualize a posição das balas e verifique as colisões com outros objetos, etc.
        for (int i = 0; i < num_bullets; i++)
        {
            // Atualize a posição Y da bala de acordo com a velocidade
            bullets[i].set_y(bullets[i].y() - BULLET_SPEED);
            bullets[i].set_bg_priority(1);
            // Verifique se a bala saiu da tela
            if (bullets[i].y() < -100)
            {
                // Remova a bala do vetor de balas e diminua o número de balas
                bullets.erase(bullets.begin() + i);
                num_bullets--;
            }
        }

        // Reinicie o contador de balas quando atingir o número máximo de balas
        if (num_bullets == MAX_BULLETS)
        {
            // Reinicie o contador de balas para 0
            num_bullets = 0;
        }
    }

    return 0;
}
