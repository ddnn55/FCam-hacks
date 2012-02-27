params = {
   exposure: 50000,
   gain: 1.0,
   focusDistance: 0.2,
   whiteBalance: 4100,
};


function setParams(value) {
  salt = new Date();
  $('#buffer0').attr('src', '/expose/'+params.exposure+'/'+params.gain+'/'+params.focusDistance+'/'+params.whiteBalance+'/'+salt);
};



window.onload = function() {
  var gui = new dat.GUI({width: '40'});
  gui.add(params, 'exposure', 0, 500000).onFinishChange(setParams);
  gui.add(params, 'gain', 1.0, 32.0).onFinishChange(setParams);
  gui.add(params, 'focusDistance', 0.05, 1.0).onFinishChange(setParams);
  gui.add(params, 'whiteBalance', 0, 6000).onFinishChange(setParams);

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


