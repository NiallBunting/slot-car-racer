// This is some very messy code, that I have designed just as some spike work
// for my idea how to control the slot car.
//
// I shall clean it up at some point i suppose.
var game = {
	p_canvas: 0,
	p_ctx: 0,

	init: function(){
		this.p_canvas=document.getElementById("mycan");
		this.p_ctx=this.p_canvas.getContext("2d");
		requestAnimationFrame(game.loop);
	},


	loop: function(){
		game.update();
		game.draw();
		requestAnimationFrame(game.loop);
	},

	update: function(){
		car.update();
	},

	// The top-level draw function
	draw: function(){
		this.p_ctx.clearRect(0, 0, this.p_ctx.canvas.width, this.p_ctx.canvas.height);
		car.draw(this.p_ctx);

		this.p_ctx.fillRect(100, 200, 10, 10);
		this.p_ctx.fillRect(300, 200, 10, 10);
		this.p_ctx.beginPath();
		this.p_ctx.moveTo(100, 100);
		this.p_ctx.lineTo(300, 100);
		this.p_ctx.stroke();
		this.p_ctx.beginPath();
		this.p_ctx.moveTo(100, 300);
		this.p_ctx.lineTo(300, 300);
		this.p_ctx.stroke();
		this.p_ctx.beginPath();
		this.p_ctx.arc(100, 200, 100, 1.6, 4.7);
		this.p_ctx.stroke();
		this.p_ctx.beginPath();
		this.p_ctx.arc(300, 200, 100, -1.6, 1.6);
		this.p_ctx.stroke();

	},

	// Takes current positon of the car
	// Returns the correction to velocity to keep on track
	trackCorrection: function(x, y, px, py){
		if(x < 100){
			// left side circle
			// x = 100, y = 200
			var nx = x + px;
			var ny = y + py;
			
			var plen = Math.sqrt(((x - nx) * (x - nx)) + ((y - ny) * (y - ny)));
			var points = this.intersect(100, 200, 100, x, y, plen);
			return [points[0] - nx, points[2] - ny];
		}else if(x > 100 && x < 300){
			// straights
			var pref_y = 0;
			if(y < 200){
				//top track
				pref_y = 100;
			} else {
				//bottom track
				pref_y = 300;
			}
			return [0, pref_y-(y+py)];

		}else{
			// right side circle
			// x = 300, y = 200
			var nx = x + px;
			var ny = y + py;
			
			var plen = Math.sqrt(((x - nx) * (x - nx)) + ((y - ny) * (y - ny)));
			var points = this.intersect(300, 200, 100, x, y, plen);
			return [points[0] - nx, points[2] - ny];
		}
		return [0,0];
	},

	// Works out the positons a circle intersescts
	intersect: function(x0, y0, r0, x1, y1, r1){
		// C -> JS of code found here.
		//http://paulbourke.net/geometry/circlesphere/
		//http://paulbourke.net/geometry/circlesphere/tvoght.c
		var dx = x1 - x0;
		var dy = y1 - y0;
		var d = Math.hypot(dx, dy);
		if(d > (r0 + r1)){
			return 0;
		}else if(d < Math.abs(r0 - r1)){
			return 0
		}

		var a = ((r0*r0) - (r1*r1) + (d*d)) / (2.0 * d) ;
		var x2 = x0 + (dx * (a/d));
		var y2 = y0 + (dy * (a/d));
		var h = Math.sqrt((r0*r0) - (a*a));
		var rx = -dy * (h/d);
		var ry = dx * (h/d);

		var xi = x2 + rx;
		var xi_ = x2 - rx;
		var yi = y2 + ry;
		var yi_ = y2 - ry;

		return [xi , xi_, yi, yi_];
	},

};

// To control the movement of the car.
var car = {
	p_x: 110,
	p_y: 100,
	p_px: 20,
	p_py: 0,
	p_fail: 0,

	// Draws the car and expected position with circle.
	draw: function(ctx){
		ctx.fillRect(this.p_x, this.p_y, -10, -5);

		ctx.beginPath();
		ctx.arc(this.p_x + this.p_px, this.p_y + this.p_py, 2, 0, 77777);
		ctx.fill();

		ctx.beginPath();
		ctx.moveTo(this.p_x, this.p_y);
		ctx.lineTo(this.p_x + this.p_px, this.p_y + this.p_py);
		ctx.stroke();
		control.draw(ctx);
	},

	// Updates the position of the car
	// Checks for loss if has to do a large correction.
	update: function(){
		this.p_x += this.p_px;
		this.p_y += this.p_py;
		this.p_px *= 0.89 + ( Math.random() * 0.05 );
		this.p_py *= 0.89 + ( Math.random() * 0.05 );
		var correct = game.trackCorrection(this.p_x , this.p_y, this.p_px, this.p_py);
		this.p_px += correct[0];// + this.p_x;
		this.p_py += correct[1];// + this.p_y;
		if(Math.abs(correct[0] + correct[1]) > 5){
			console.log("loss");
			this.p_x = this.p_y = this.p_px = this.p_py = 0;
			this.p_fail = 1;
		}
		//calc the speed
		var speed = Math.hypot(this.p_px, this.p_py);
		//returns the amount of power to add
		var volt = control.update(this.p_x, this.p_y, speed, this.p_fail);
		//keep in bounds
		if (volt < 1 || volt > 2){
			volt = 1;
		}
		this.p_px *= volt[0];
		this.p_py *= volt[1];
	},

	reset: function(){
		this.p_x= 110;
		this.p_y= 100;
		this.p_px= 20;
		this.p_py= 0;
		this.p_fail= 0;
	},

};

// The part doing the contol.
var control = {
	p_maxspeed: 14,
	p_maxspeedset: 0,
	p_startpos: 0,
	p_startflag: 0,
	p_startcounter: 0,
	p_pointlist: [],
	p_lastpos: [0,0],
	p_time: 0,

	update: function(x, y, speed, fail){
		var init = this.init(x, y, speed, fail);
		if(init[0] != -1){return init;}

		//calculate how many boxes car is in.
		var riskyplace = 0;
		for(var i = 0; i < this.p_pointlist.length; i++){
			if(this.inBox(x, y, this.p_pointlist[i][0], this.p_pointlist[i][1])){riskyplace++;}
		}

		//what to do if the car has failed.
		if(fail){
			this.addfailbox();
			this.p_startcounter = 0;
			car.reset();
		}else{
			//keep out of box if over 6
			if(riskyplace < 6){
				this.p_lastpos = [x, y];
			}
		}

		//Deletes a square every one in a hundred.
		if(Math.random() > 0.99){
				this.p_pointlist.splice(Math.round(Math.random() * this.p_pointlist.length), 1);
				console.log("Deleted one");
		}

		//speed changers
		//console.log(speed + " " + this.p_startcounter);
		if(riskyplace > 2){
			if (speed < this.p_maxspeed){
				return[1.1, 1.1];
			}else{
				return [1,1];
			}
		}else if(riskyplace > 0){
			return [1.15, 1.15];
		}else{
			return [1.5, 1.5];
		}
		

		return [1,1];
	},

	//Adds a area where the car has fallen off
	addfailbox: function(){
		//make sure not adding in the top corner after fail
		if(this.p_lastpos[0] == 0 || this.p_lastpos[1] == 0){
			return 0;
		}
		this.p_pointlist[this.p_pointlist.length] = this.p_lastpos;
	},

	draw: function(ctx){
		ctx.strokeStyle="#FF0000";
		this.drawBox(this.p_startpos[0], this.p_startpos[1], ctx);
		ctx.strokeStyle="#000000";
		for(var i = 0; i < this.p_pointlist.length; i++){
			this.drawBox(this.p_pointlist[i][0], this.p_pointlist[i][1], ctx);
		}
	},
	//Draws lines around the area.
	drawBox: function(x, y, ctx){
		ctx.beginPath();
		ctx.moveTo(x + 10, y + 10);
		ctx.lineTo(x + 10, y - 10);
		ctx.stroke();

		ctx.beginPath();
		ctx.moveTo(x - 10, y - 10);
		ctx.lineTo(x + 10, y - 10);
		ctx.stroke();

		ctx.beginPath();
		ctx.moveTo(x + 10, y + 10);
		ctx.lineTo(x - 10, y + 10);
		ctx.stroke();

		ctx.beginPath();
		ctx.moveTo(x - 10, y - 10);
		ctx.lineTo(x - 10, y + 10);
		ctx.stroke();

	},

	init: function(x, y, speed, fail){
		// Check if has start position
		if(this.p_startpos == 0){
			this.p_startpos = [x, y];
		}
		//updates when passed start line
		this.updateStartCounter(x, y);

		//keeps going till max speed
		var maxspeed = this.setMaxSpeed(x, y, speed, fail);
		if(maxspeed[0] != -1){return maxspeed;}else{return [-1,0];}
	},

	setMaxSpeed: function(x, y, speed, fail){
		//Here each time passes start increases the max speed
		if(this.p_maxspeedset != -1){
			if(fail == 0){
				//check if passed start again
				//if so increase speed max and send around
				if(this.p_startcounter > this.p_maxspeedset){
					this.p_maxspeedset = this.p_startcounter;
					this.p_maxspeed += 0.3;
				}
				//keeps speed at max whilst going around
				if(speed < this.p_maxspeed){
					return [1.1, 1.1];
				}else{
					return [1, 1];
				}
			}else{
				//fininsh this part
				this.p_maxspeedset = -1;
				//remove one so safe
				this.p_maxspeed -= 1;
				//reset the car
				car.reset();
				console.log("Final max: " + this.p_maxspeed);
			}
		}
		return [-1];

	},

	updateStartCounter: function(x, y){
		//startflag - 0 not start, 1 start
		var flag = this.inBox(x, y, this.p_startpos[0], this.p_startpos[1]);
		//Checks if leaving box
		if(this.p_startflag && !flag){
			this.p_startcounter += 1;
			//If not first lap show laptime.
			if(this.p_startcounter > 1){
				console.log("Lap time: " + (Date.now() - this.p_time));
			}
			//Reset the counter for the laptime.
			this.p_time = Date.now();
		}
		this.p_startflag = flag;
	},

	//checks if the car is within a certain range.
	inBox: function(x, y, xflag, yflag){
		if(x > xflag - 20 && x < xflag + 20 && y > yflag - 20 && y < yflag + 20){
			return true;
		}
		return false;
	},

};
