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

	trackCorrection: function(x, y, px, py){
		if(x < 100){
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
			// x = 300, y = 200
			var nx = x + px;
			var ny = y + py;
			
			var plen = Math.sqrt(((x - nx) * (x - nx)) + ((y - ny) * (y - ny)));
			var points = this.intersect(300, 200, 100, x, y, plen);
			return [points[0] - nx, points[2] - ny];
		}
		return [0,0];
	},

	intersect: function(x0, y0, r0, x1, y1, r1){
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

var car = {
	p_x: 100,
	p_y: 100,
	p_px: 10,
	p_py: 4,

	draw: function(ctx){
		ctx.fillRect(this.p_x, this.p_y, -10, -5);

		ctx.beginPath();
		ctx.arc(this.p_x + this.p_px, this.p_y + this.p_py, 2, 0, 77777);
		ctx.fill();

		ctx.beginPath();
		ctx.moveTo(this.p_x, this.p_y);
		ctx.lineTo(this.p_x + this.p_px, this.p_y + this.p_py);
		ctx.stroke();
	},

	update: function(){
		this.p_x += this.p_px;
		this.p_y += this.p_py;
		this.p_px *= 0.99;
		this.p_py *= 0.99;
		var correct = game.trackCorrection(this.p_x , this.p_y, this.p_px, this.p_py);
		this.p_px += correct[0];// + this.p_x;
		this.p_py += correct[1];// + this.p_y;
	//	if(Math.abs(correct[0] + correct[1]) > 5){
	//		this.p_x = this.p_y = this.p_px = this.p_py = 0;
	//	}
		control.update(this.p_x, this.p_y);
	},

};

var control = {
	p_volts: 0,
	p_speed: 0,
	p_locx: 0,
	p_locy: 0,
	update: function(x, y){

	},

};
