<style>
	
#edit-field-phone-no-und-0-value,#edit-field-email-und-0-email,#edit-title,#edit-field-other-occupation-und-0-value{ width: 240px !important;}
.image-widget-data .form-submit{margin-top: -26px;margin-right: -50px; float: right;}
.form-item{ margin-bottom: 0px;
    margin-top: 0px;}
	</style>
	<!--
	<script>
		

 

$(document).ready(function(){
    $("select.form-select").change(function(){
        var selectedCountry = $(".form-select option:selected").val();
        alert("You have selected the country - " + selectedCountry);
    });
});

		</script>-->
<?php 
//$form['actions']['submit']['#value']='Save and Add more';

$form['body']['und']['0']['value']['#attributes']['class']=array('area-width');


?>
  <?php print drupal_render_children($form); ?>
<?php //print drupal_render($form['title']); ?>



		<?php   print views_embed_view('live_blog_data_node_form','block',$form['field_select_live_blog']['und']['#default_value'])?>
		 

