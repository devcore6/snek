#include <vga.h>
#include <sleep.h>
#include <kb.h>

uint16_t *snake_data = (uint16_t*)0x200000; // 2MB in or something cause I'm not gonna implement heap and paging for this

size_t snake_length = 1;
size_t snake_growth = 5;

extern "C" {
	uint8_t snake_direction = 0;
	bool dead = true;
	bool ai = true;
}

uint16_t apple = 0;

uint8_t snake_speed = 0;

// 0 = North
// 1 = West
// 2 = South
// 3 = East

void spawn_apple() {
	tty_putat(' ', (apple & 0xFF00) >> 8, apple & 0xFF); // Undraw old apple
	while(true) {
		apple = ((rand() % VGA_WIDTH) << 8) | (rand() % VGA_HEIGHT);
		bool b = true;
		for(size_t i = 0; i < snake_length; i++)
			if(apple == snake_data[i]) {
				b = false;
				break;
			}
		if(b) break;
	}
	tty_setcolor(VGA_GREEN, VGA_BLACK);
	tty_putat('#', (apple & 0xFF00) >> 8, apple & 0xFF); // Draw new apple
	srand(system_timer_ms);
}

void init_gamefield() {
	memset(snake_data, 0, VGA_WIDTH * VGA_HEIGHT);
	snake_length = 5;
	snake_direction = 0;
	snake_growth = 0;
	snake_speed = 0;
	for(size_t i = 0; i < snake_length; i++) {
		uint8_t x = VGA_WIDTH / 2;
		uint8_t y = VGA_HEIGHT / 2 + i;
		snake_data[i] = (x << 8) | y;
	}
	spawn_apple();
	dead = false;
}

void draw_snake() {
	tty_setcolor(VGA_GREEN, VGA_BLACK);
	tty_putat('#', (apple & 0xFF00) >> 8, apple & 0xFF);
	tty_setcolor(VGA_WHITE, VGA_BLACK);
	tty_putat('#', (snake_data[0] & 0xFF00) >> 8, snake_data[0] & 0xFF);
	tty_setcolor(VGA_DGRAY, VGA_BLACK);
	for(size_t i = 1; i < snake_length; i++) {
		tty_putat('#', (snake_data[i] & 0xFF00) >> 8, snake_data[i] & 0xFF);
	}
}

void die() {
	tty_init();
	tty_write("You lost kek\n");
	dead = true;
}

uint8_t last_column = 0;
uint8_t last_dir = 0;

void do_ai() {
	switch(snake_direction) {
		case 0: { // Going north - should turn east after
			uint8_t old_x = (snake_data[0] & 0xFF00) >> 8;
			uint8_t old_y = (snake_data[0] & 0x00FF);
			last_column = old_x;
			last_dir = 0;
			if(old_y == 0) {
				snake_direction = 3;
			}
			break;
		}
		case 1: { // Going West - should turn north after
			uint8_t old_x = (snake_data[0] & 0xFF00) >> 8;
			if(old_x == 0) {
				snake_direction = 0;
			}
			break;
		}
		case 2: { // Going South - should turn east/west after
			uint8_t old_x = (snake_data[0] & 0xFF00) >> 8;
			uint8_t old_y = (snake_data[0] & 0x00FF);
			last_column = old_x;
			last_dir = 2;
			if(old_x == VGA_WIDTH - 1) {
				if(old_y == VGA_HEIGHT - 1) {
					snake_direction = 1; // West if last column
				}
			} else {
				if(old_y == VGA_HEIGHT - 2) {
					snake_direction = 3; // Otherwise east
				}
			}
			break;
		}
		case 3: { // Going East - should turn north/south after
			uint8_t old_x = (snake_data[0] & 0xFF00) >> 8;
			if(old_x != last_column) {
				if(last_dir == 2) snake_direction = 0; // Go north if previously going south
				else snake_direction = 2; // Otherwise go south now
			}
			break;
		}
	}
}

bool update_snake() {
	// First backup head position
	uint16_t head_pos = snake_data[0];

	// Update head position
	switch(snake_direction) {
		case 0: {
			uint8_t old_x = (snake_data[0] & 0xFF00) >> 8;
			uint8_t old_y = (snake_data[0] & 0x00FF);
			old_y--;
			if(old_y > VGA_HEIGHT) {
				die();
				return false;
			}
			else snake_data[0] = (old_x << 8) | (old_y);
			break;
		}
		case 1: {
			uint8_t old_x = (snake_data[0] & 0xFF00) >> 8;
			uint8_t old_y = (snake_data[0] & 0x00FF);
			old_x--;
			if(old_x > VGA_WIDTH) {
				die();
				return false;
			}
			else snake_data[0] = (old_x << 8) | (old_y);
			break;
		}
		case 2: {
			uint8_t old_x = (snake_data[0] & 0xFF00) >> 8;
			uint8_t old_y = (snake_data[0] & 0x00FF);
			old_y++;
			if(old_y > VGA_HEIGHT) {
				die();
				return false;
			}
			else snake_data[0] = (old_x << 8) | (old_y);
			break;
		}
		case 3: {
			uint8_t old_x = (snake_data[0] & 0xFF00) >> 8;
			uint8_t old_y = (snake_data[0] & 0x00FF);
			old_x++;
			if(old_x > VGA_WIDTH) {
				die();
				return false;
			}
			else snake_data[0] = (old_x << 8) | (old_y);
			break;
		}
	}

	// Check head collision with body
	for(size_t i = 1; i < snake_length; i++)
		if(snake_data[0] == snake_data[i]) {
			die();
			return false;
		}

	// Check for head collision with apple
	if(snake_data[0] == apple) {
		snake_growth += 5;
		if(snake_speed < 50) snake_speed++;
	}

	// Draw new head
	tty_setcolor(VGA_WHITE, VGA_BLACK);
	tty_putat('#', (snake_data[0] & 0xFF00) >> 8, snake_data[0] & 0xFF);

	// Replace old head with snake body character
	tty_setcolor(VGA_DGRAY, VGA_BLACK);
	tty_putat('#', (head_pos & 0xFF00) >> 8, head_pos & 0xFF);

	// Update body position
	uint16_t snake_end = snake_data[snake_length - 1];
	for(size_t i = snake_length; i > 1; i--)
		snake_data[i] = snake_data[i - 1];

	snake_data[1] = head_pos;

	// Increase length if we have to
	if(snake_growth) {
		snake_data[snake_length] = snake_end;
		snake_length++;
		snake_growth--;
	} else {
		tty_putat(' ', (snake_end & 0xFF00) >> 8, snake_end & 0xFF); // Remove snake end if we didn't grow
	}

	// Spawn new apple
	if(snake_data[0] == apple) {
		spawn_apple();
	}
	return true;
}

extern "C" void kernel_main() {
	while(true) {
		tty_init();
		init_gamefield();

		while(true) {
			if(ai) do_ai();
			if(!update_snake()) break;
			//tty_init();
			//draw_snake();
			if(ai) sleep(1); // Make the AI run faster so I don't have to wait as long
			else sleep(60 - snake_speed);
		}

		while(!_enter) {
			asm volatile ("hlt"); // Wait until we hit enter to start new life
		}
	}

	// Sleep in low power mode instead of returning once kernel is done
	while(true) {
		asm volatile("hlt");
	}
}
