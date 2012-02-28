params = {
   exposurePower: 17,
   gain: 1.0,
   diopter: 0.0,
   whiteBalance: 4100,
};


function setParams(value) {
  salt = new Date();
  $('#buffer0').attr('src', '/expose/'+Math.pow(2, params.exposurePower)+'/'+params.gain+'/'+params.diopter+'/'+params.whiteBalance+'/'+salt);
};



window.onload = function() {
  var gui = new dat.GUI({width: '40'});
  gui.add(params, 'exposurePower', 0.0, 21.0).onFinishChange(setParams);
  gui.add(params, 'gain', 1.0, 32.0).onFinishChange(setParams);
  gui.add(params, 'diopter', 0.0, 10.0).onFinishChange(setParams);
  gui.add(params, 'whiteBalance', 0, 7000).onFinishChange(setParams);


  setParams();
  //$('#buffer0').css('display': 'none');

/*  $('#buffer0').load(function() {
    $('#buffer1').css('display', 'none');
    $('#buffer0').css('display', 'inline');
    setTimeout('loadBuffer(1)', 300);
  });
  $('#buffer1').load(function() {
    $('#buffer0').css('display', 'none');
    $('#buffer1').css('display', 'inline');
    setTimeout('loadBuffer(0)', 300);
  });*/

  //window.setTimeout('loadBuffer(0)', 0);
};


