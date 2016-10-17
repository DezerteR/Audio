#include "Includes.hpp"
#include "Utils.hpp"
#include "Timer.hpp"
#include "Constants.hpp"

#include <AL/al.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/gtx/matrix_major_storage.hpp>
#include <glm/gtx/perpendicular.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtx/optimum_pow.hpp>

#include "IAudio.hpp"


void wait(double mces){
	Timer<double, 1000, 1> timer;
	while(mces > 0.0){
		mces -= timer();
	}
	// cerr<<"."<<endl;
}

/// music player
void test_1(){
	Timer<double, 1000, 1> timer;
	cerr << "A";
	audio->music.play();
	cerr << "B";

	for(int i = 3; i>0; i--){
		cerr<<i<<".."<<endl;
		double timeLeft = 1000.0;

		audio->music.next();

		while(timeLeft > 0.0){
			timeLeft -= timer();
		}
	}

}

/// sound player
void test_2(){
	Timer<double, 1000, 1> timer;

	for(int i = 20; i>0; i--){
		cerr<<i<<".."<<endl;
		double timeLeft = 100.0;
		// SoundPlayer3D::get(i).play();

		while(timeLeft > 0.0){
			timeLeft -= timer();
		}
		// SoundSource::update(100);
	}
}

/// 3D effects
void test_3(){
	auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine RNG(seed);
	std::uniform_real_distribution <float> dis(-10, 10);
	std::uniform_real_distribution <double> dt(150, 300);
	std::uniform_int_distribution <u32> tr(0, 15);

	for(u32 i = 0; i < 50; i++){
		// SoundPlayer3D::get(tr(RNG))
		// 	.position(glm::vec4(dis(RNG), dis(RNG), 0, 1))
		// 	.attenuation(0.1)
		// 	// .pitch(2)
		// 	.play();
		wait(dt(RNG));
	}

	wait(1800);

	// SoundSource::update(100);
}

/// looped source
void test_4(){
	u32 lifetime = 1500;
	// SoundPlayer3D::get(1)
	// 	.lifetime(lifetime*5)
	// 	.position({0,100,0,1})
	// 	.volume(1)
	// 	.attenuation(0.2f)
	// 	.play()
	// 	;
	wait(450);
	// SoundPlayer3D::get(1)
	// 	.lifetime(lifetime*5)
	// 	.position({0,-100,0,1})
	// 	.attenuation(0.2f)
	// 	.volume(1)
	// 	.play()
	// 	;
	// Listener::position({0,0,0,0});
	// Listener::velocity({0,0,0,0});
	// Listener::orientation({1,0,0,0}, {0,0,1,0});
	wait(8000);
	float angle = 0;

	for(u32 i=0; i<lifetime; i++){
		// angle += 30/500.f;
		glm::vec4 listenerForward = glm::rotate(angle, glm::vec3(0,0,1)) * glm::vec4(1,0,0,0);
		// Listener::orientation(listenerForward, {0,0,1,0});

		wait(5);
		// SoundSource::update(5);
	}

}

/// two 3d musicPlayers
void test_5(){
	// StreamedPlayer3D left;
	// left.loadFromDirectory("../res/audio/music/");
	// left.next();
	// left.position({0, -10,0,0})
	// 	.attenuation(0.2)
	// 	.volume(1.0f)
	// 	.play();
	//
	// StreamedPlayer3D right;
	// right.loadFromDirectory("../res/audio/music/");
	// right.next();
	// right.next();
	// right.next();
	// right.position({0, 10,0,0})
	// 	.attenuation(0.2)
	// 	.volume(1.0f)
	// 	.play();

	wait(8500);
}

int main(){
	audio = make_unique<IAudio>();
	cerr << "0";
	audio->init();
	cerr << "1";

	test_1();
	// test_2();
	// test_3();
	// test_4();
	// test_5();

	return 0;
}
