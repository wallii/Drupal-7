<?php
/**
 * The ZeeNews Hindi CategoryMigration
 */
class ZNHCategory extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');
        // $this->dependencies = array('ZeeNewsCategory');
        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'section_id' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'cat',
                    'description' => 'catid from legacy table category',
                )
            ),
            MigrateDestinationTerm::getKeySchema()
        );

        $query = Database::getConnection('zeehindi161', 'for_migration')
            ->select('section_master', 'cat');
        $query->fields('cat', array('section_id','section_name'));
     //   $query->range(0, 1);
        // Create a MigrateSource object, which manages retrieving the input data.
        $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationTerm('hindi_sections');
        $this->addFieldMapping('name', 'section_name');
        $this->addFieldMapping('description', 'section_id');
        //$this->addFieldMapping('parent')->defaultvalue('2433');
        //$this->addFieldMapping('parent_name')->defaultvalue('State News');
        //$this->addFieldMapping('parent_name','section_domain');
        //        ->sourceMigration('Category')
        //       ->arguments(array('source_type' => 'tid'));

        // We conditionally DNM these fields, so your field mappings will be clean
        // whether or not you have path and or pathauto enabled
        if (module_exists('path')) {
            $this->addFieldMapping('path')
                ->issueGroup(t('DNM'));
            if (module_exists('pathauto')) {
                $this->addFieldMapping('pathauto')
                    ->issueGroup(t('DNM'));
            }
        }
    }
    public function prepareRow($row) {

    }
}


/**
 * The ZeeNews Hindi Node Migration
 */

class ZNHNode extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');
        $options = array('track_changes' => 1);
        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'story_id' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'znews',
                    'description' => 'catid from legacy table category',
                )
            ),
            MigrateDestinationNode::getKeySchema()
        );

      //  $query = Database::getConnection('zeehindi161', 'for_migration')
      //      ->select('news_master', 'znews');
      //  $query->fields('znews', array('story_id','story_title','sections','domain_id','sections','image_courtesy','tag','author_id','nsource','news_content','story_summary','story_date','LastUpdate','url','story_image','story_etitle'));
      //  $query->orderBy('story_id', 'asc');
      //  $query->range(0, 1);

        $query = "SELECT story_id,story_title,sections,domain_id,sections,image_courtesy,tag,author_id,NSource,news_content,story_summary,story_date,LastUpdate,url,story_image,story_etitle,ourl,ifile FROM news_master";
        $count_query = "SELECT COUNT(*) FROM news_master";
//        $query = "SELECT story_id,story_title,sections,domain_id,sections,image_courtesy FROM news_master";
        $fields = array(
            'story_id' => t('story_id'),
            'story_title' => t('story_title ID'),
            'sections' => t('sections sections'),
            'domain_id' => t('domain_id ID'),
            'sections' => t('sections ID'),
            'image_courtesy' => t('image_courtesy ID'),
            'tag' => t('tag ID'),
            'author_id' => t('author_id ID'),
            'nsource' => t('nsource'),
            'news_content' => t('news_content ID'),
            'story_summary' => t('story_summary ID'),
            'story_date' => t('story_date ID'),
            'LastUpdate' => t('LastUpdate ID'),
            'url' => t('url ID'),
            'story_image' => t('story_image ID'),
            'story_etitle' => t('story_etitle ID'),
            'ourl' => t('ourl ID'),
            'ifile' => t('Image File Name'),
        );
        $this->source = new MigrateSourceMSSQL(array(
            'servername' => '180.179.93.214',
            'username' => 'sa',
            'password' => 'P@ssw0rd',
            'database' => 'ZeeNews_Hindi',
        ), $query, $count_query, $fields);
        //  $query->addExpression('GROUP_CONCAT(DISTINCT section)', 'section');

        // Create a MigrateSource object, which manages retrieving the input data.
//        $this->source = new MigrateSourceMSSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationNode('hindi_news', array('text_format' => 'full_html'));
        //$this->addFieldMapping('nid', 'story_id');
        $this->addFieldMapping('title', 'story_title');
        $this->addFieldMapping('field_author', 'author_id')
            ->sourceMigration('ZNHAuthor')
            ->arguments(array('source_type' => 'uid'));
        $this->addFieldMapping('field_hi_section', 'sections')
            ->separator(',')
            ->sourceMigration('ZNHCategory')
            ->arguments(array('source_type' => 'tid'));
        $this->addFieldMapping('field_tags', 'tag');
        $this->addFieldMapping('field_news_source', 'nsource')
            ->sourceMigration(array('ZNSource'))
            ->arguments(array('source_type' => 'tid'));
        $this->addFieldMapping('field_english_title', 'story_etitle');
        $this->addFieldMapping('field_home_title', 'story_title');
        $this->addFieldMapping('field_image_caption', 'image_courtesy');
        $this->addFieldMapping('field_oldid', 'story_id');
        $this->addFieldMapping('body', 'news_content');
        $this->addFieldMapping('body:summary', 'story_summary');
        $this->addFieldMapping('is_new')->defaultValue(TRUE);
        $this->addFieldMapping('uid')->defaultValue(1);
        //$this->addFieldMapping('language')->defaultValue('en');
        $this->addFieldMapping('created', 'story_date');
        $this->addFieldMapping('changed', 'LastUpdate');
        $this->addFieldMapping('pathauto')->defaultValue("0");
        $this->addFieldMapping('field_show_home_image')->defaultValue(0);
        $this->addFieldMapping('path','url');

       //Images
        $this->addFieldMapping('field_image', 'story_image');
        $this->addFieldMapping('field_image:source_dir')
            ->defaultValue('http://origin.znn.india.com/hindi');
        $this->addFieldMapping('field_image:destination_file','ifile');

        module_load_include('inc','pathauto','pathauto');

    }

    public function prepareRow($row) {

        if (strlen(trim($row->tag)) < 4 )
        {
            $row->tag = null;
        }
        if ($row->story_etitle == 'test')
        {
            $row->story_etitle = null;
        }
        if ($row->author_id==0){$row->author_id=null;}
        $row->news_content =  str_replace("<br>", "<br/>", $row->news_content);
        $row->news_content =  str_replace("<br/>", "\n", $row->news_content);
        $row->news_content =  str_replace("\n\n", "\n", $row->news_content);
        if($row->story_id <191622)
        {
            $row->url=$row->ourl;
        }
        else{
            $vFiled = pathinfo($row->ifile);
            $row->ifile = pathauto_cleanstring($vFiled['filename']) .'.'.$vFiled['extension'];


            // $row->url='hindi/'.$row->url;
            if ($vFiled['filename'] =='spacer')
            {
                //remove while importing images
                  $row->story_image = null;
            }

                $row->image_courtesy = null;

        }



    }
    public function prepare($node, stdClass $row) {


        $keywords = explode(',', $row->tag);
        static $vid;
        if (!isset($vid)) {
            $vocabs = taxonomy_vocabulary_load_multiple(array(), array('machine_name' => 'tags'));
            $vids = array_keys($vocabs);
            $vid = reset($vids);
        }
        foreach ($keywords as $index => $keyword) {

            $keyword=trim($keyword);
            if (!empty($keyword))
            {
                $matches = taxonomy_get_term_by_name($keyword);
                if (empty($matches)) {
                    $term = new stdClass;
                    $term->name = $keyword;
                    $term->vocabulary_machine_name = 'tags';
                    $term->vid = $vid;
                    taxonomy_term_save($term);
                }
                else {
                    $term = reset($matches);
                }
                $node->field_tags[LANGUAGE_NONE][$index]['tid'] = $term->tid;

            }
        }
             if($row->story_id >191622)
        {
            $html = str_get_html($node->body[LANGUAGE_NONE][0]['value']);

            $total_img = count($html->find("img"));
            if ($total_img > 0) {
                for ($i = 0; $i < $total_img; $i++) {
                    $element=$html->find("img", $i);
                    $src = $html->find("img", $i)->src;
                        $url = $src;
                        $url_info = parse_url($url);
                        $url_path_info = pathinfo($url_info['path']);

                        //This helps with filenames with spaces
                        $url = $url_info['scheme'] . '://' . $url_info['host']  . $url_path_info['dirname'] .'/'. rawurlencode($url_path_info['basename']);
                    $vFiled = pathinfo($url_path_info['basename']);
                        //$dest='public://inline/' . str_replace(' ','_',$url_path_info['basename']);
                    $dest='public://inline/' . pathauto_cleanstring($vFiled['filename']) .'.'.$vFiled['extension'];;
                    if($vFiled['filename'] == 'spacer')
                    {
                        $element->outertext ='';
                    }
                    else
                    {
                        //print $url;
                        $remoteDocPath = strtolower($url);
                        $doc = system_retrieve_file($remoteDocPath,$dest, true, FILE_EXISTS_RENAME);
                        //  $mfiles = drupal_add_existing_file($doc);
                        if ( strlen($doc->uri) > 10){
                            $html->find("img", $i)->src = str_replace('public://inline/','http://sth.india.com/hindi/sites/default/files/inline/', $doc->uri);
                        }
                        else{
                            $element->outertext ='';
                        }
                    }

                }

                $node->body[LANGUAGE_NONE][0]['value'] = $html->save();
                $node->body[LANGUAGE_NONE][0]['value']=str_replace('<img','<img class="imglft" ',$node->body[LANGUAGE_NONE][0]['value']);
            }

        }
        else
        {
            //Remove while importing with images
            $html = str_get_html($node->body[LANGUAGE_NONE][0]['value']);

            $total_img = count($html->find("img"));
            if ($total_img > 0) {
                for ($i = 0; $i < $total_img; $i++) {
                    $element=$html->find("img", $i);
                    $element->outertext ='';
                    $node->body[LANGUAGE_NONE][0]['value'] = $html->save();
                }
            }
        }
    }

}


/**
 * The ZeeNews Blog and Exclusive Node Migration
 */

class ZNHBlogs extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');
        $options = array('track_changes' => 1);
        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'blog_id' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'znews',
                    'description' => 'catid from legacy table category',
                )
            ),
            MigrateDestinationNode::getKeySchema()
        );

        $query = "select blog_id,blog_caption,blog_title,blog_summary,author_id,blog_date,blog_text,blog_etitle,tag,blog_status from blog_master";
        $count_query = "SELECT COUNT(*) FROM blog_master";
//        $query = "SELECT story_id,story_title,sections,domain_id,sections,image_courtesy FROM news_master";
        $fields = array(
            'blog_id' => t('blog_id'),
            'blog_caption' => t('Blog_Caption'),
            'blog_title' => t('Blog_Title'),
            'blog_summary' => t('Blog_Summary ID'),
            'author_id' => t('author_id ID'),
            'blog_date' => t('Blog_Date ID'),
            'blog_text' => t('Blog_Text'),
            'blog_etitle' => t('blog_etitle ID'),
            'tag' => t('tag ID'),
            'blog_status' => t('URL'),
        );
        $this->source = new MigrateSourceMSSQL(array(
            'servername' => '180.179.93.214',
            'username' => 'sa',
            'password' => 'P@ssw0rd',
            'database' => 'ZeeNews_Hindi',
        ), $query, $count_query, $fields);

        //  $query->addExpression('GROUP_CONCAT(DISTINCT section)', 'section');

        // Create a MigrateSource object, which manages retrieving the input data.
        $this->destination = new MigrateDestinationNode('hindi_news', array('text_format' => 'full_html'));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        //$this->addFieldMapping('nid', 'story_id');
        $this->addFieldMapping('title', 'blog_title');
        $this->addFieldMapping('field_author', 'author_id')
            ->sourceMigration('ZNHAuthor')
            ->arguments(array('source_type' => 'uid'));
        $this->addFieldMapping('field_tags', 'tag');
        $this->addFieldMapping('field_news_source')->defaultValue(1);
        $this->addFieldMapping('field_home_title', 'blog_title');
        $this->addFieldMapping('field_english_title', 'blog_etitle');
        $this->addFieldMapping('field_oldid', 'blog_id');
        $this->addFieldMapping('body', 'blog_text');
        $this->addFieldMapping('body:summary', 'blog_summary');
        $this->addFieldMapping('is_new')->defaultValue(TRUE);
        $this->addFieldMapping('uid')->defaultValue(1);
        //$this->addFieldMapping('language')->defaultValue('en');
        $this->addFieldMapping('created', 'blog_date');
        $this->addFieldMapping('changed', 'blog_date');
        $this->addFieldMapping('pathauto')->defaultValue("0");
        $this->addFieldMapping('field_show_home_image')->defaultValue(0);
        $this->addFieldMapping('path','blog_status');
        $this->addFieldMapping('field_is_review')->defaultValue(1);
        $this->addFieldMapping('field_image_caption','blog_caption');

    }

    public function prepareRow($row) {

        $row->blog_status='blog/'. pathauto_cleanstring($row->blog_etitle) . '_' .$row->blog_id .'.html' ;
    }

    public function prepare($node, stdClass $row) {
        $keywords = explode(',', $row->tag);
        static $vid;
        if (!isset($vid)) {
            $vocabs = taxonomy_vocabulary_load_multiple(array(), array('machine_name' => 'tags'));
            $vids = array_keys($vocabs);
            $vid = reset($vids);
        }
        foreach ($keywords as $index => $keyword) {

            $keyword=trim($keyword);
            if (!empty($keyword))
            {
                $matches = taxonomy_get_term_by_name($keyword);
                if (empty($matches)) {
                    $term = new stdClass;
                    $term->name = $keyword;
                    $term->vocabulary_machine_name = 'tags';
                    $term->vid = $vid;
                    taxonomy_term_save($term);
                }
                else {
                    $term = reset($matches);
                }
                $node->field_tags[LANGUAGE_NONE][$index]['tid'] = $term->tid;

            }
        }

    }

}


/**
 * The ZeeNews Authors Migration
 */
class ZNHAuthor extends ZeeNewsMigration {
    public function __construct() {

        parent::__construct();
        $this->description = t('Zee News Hindi Authors');
        $this->map = new MigrateSQLMap($this->machineName,
            array('author_id' => array(
                'type' => 'int',
                'not null' => TRUE,
                'description' => 'Account ID.'
            )
            ),
            MigrateDestinationUser::getKeySchema()
        );
        $query = Database::getConnection('zeehindi161', 'for_migration')
            ->select('authors','au')
            ->fields('au', array('author_id', 'author_name', 'author_email', 'googleid', 'author_thumb_image', 'author_twitter','vfile'));
        $query->condition('author_id', '101', '>');
        $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));
        $this->destination = new MigrateDestinationUser();


        // Our source table has two entries for 'alice', but we must have a unique
        // username in the Drupal 'users' table. dedupe() creates new, unique
        // destination values when the source field of that value already exists.
        // For example, if we're importing a user with name 'test' and a user
        // 'test' already exists in the target, we'll create a new user named
        // 'test_1'.
        // dedupe() takes the Drupal table and column for determining uniqueness.
        $this->addFieldMapping('name', 'author_name')
            ->dedupe('users', 'name');
        $this->addFieldMapping('uid', 'author_id');
        $this->addFieldMapping('status')
            ->defaultValue(1);
        $this->addFieldMapping('is_new')
            ->defaultValue(TRUE);
         $this->addFieldMapping('field_twitter', 'author_twitter');
         $this->addFieldMapping('field_google', 'googleid');

        $this->addFieldMapping('mail', 'author_email');
        $this->addFieldMapping('field_image', 'author_thumb_image');
        $this->addFieldMapping('field_image:destination_file','vfile');
        $this->addFieldMapping('field_image:source_dir')
            ->defaultValue('http://origin.znn.india.com/hindi');


        $this->addFieldMapping('pass')
            ->defaultValue('sdhjskdhskd@sdhjksds');

        $this->addFieldMapping('roles')
            ->defaultValue(DRUPAL_AUTHENTICATED_RID);
        module_load_include('inc','pathauto','pathauto');

        if (module_exists('path')) {
            $this->addFieldMapping('path')
                ->issueGroup(t('DNM'));
            if (module_exists('pathauto')) {
                $this->addFieldMapping('pathauto')
                    ->issueGroup(t('DNM'));
            }
        }

            }
    public function prepareRow($row) {
        $vFiled = pathinfo($row->vfile);
        $row->vfile = pathauto_cleanstring(strtolower($vFiled['filename'])) .'.'.$vFiled['extension'];

    }
}


/**
 * The Hindi Video Migration
 */
class ZNHVideo extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');
        //   $this->dependencies = array('ZeeNewsVideoCategory');
        $options = array('track_changes' => 1);
        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'video_id' => array(
                    'type' => 'int',
                    'not null' => false,
                    'alias' => 'zvids',
                    'description' => 'catid from legacy table category',
                )
            ),
            MigrateDestinationNode::getKeySchema()
        );

        $query = Database::getConnection('zeehindi161', 'for_migration')
            ->select('news_videos', 'zvids'); //SELECT     TOP (100) PERCENT iVediouplaod, vFilename, dUploaddate, LTRIM(RTRIM(cSection)) AS csection, cDescription, cTitle, dbo.RemoveSpecialChars(cTitle) AS url         FROM         dbo.zn_vedio_upload  WHERE     (isYoutube = 0) ORDER BY iVediouplaod
        $query->fields('zvids', array('video_id','video_title','video_desc','video_file','video_image','sectionid','video_etitle','youtubecode', 'isYoutube','video_date','url','vfile','ifile'));

        $query->orderBy('video_id', 'asc');
//        $query->range(0, 100);

        //  $query->addExpression('GROUP_CONCAT(DISTINCT section)', 'section');

        // Create a MigrateSource object, which manages retrieving the input data.
        $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationNode('videos', array('text_format' => 'full_html'));
        //$this->addFieldMapping('nid', 'ivediouplaod');
        $this->addFieldMapping('field_oldid', 'video_id');
        $this->addFieldMapping('body', 'video_desc');
        $this->addFieldMapping('title', 'video_title');
        $this->addFieldMapping('field_yt_code', 'youtubecode');
        $this->addFieldMapping('field_home_title', 'video_title');
        $this->addFieldMapping('is_new')->defaultValue(TRUE);
        $this->addFieldMapping('uid')->defaultValue(1);
        // $this->addFieldMapping('language')->defaultValue('en');
        $this->addFieldMapping('created', 'video_date');
        $this->addFieldMapping('changed', 'video_date');
        $this->addFieldMapping('field_domain')->defaultValue("8");
        $this->addFieldMapping('field_isyoutube','isYoutube');
        $this->addFieldMapping('pathauto')->defaultValue("0");
        $this->addFieldMapping('path','url');

        //   $this->addFieldMapping('field_image:file_replace')->defaultValue(MigrateFile::FILE_EXISTS_REUSE);
        // $this->addFieldmapping('field_image:preserve_files')->defaultValue(TRUE);
        $this->addFieldMapping('field_image', 'video_image');
        $this->addFieldMapping('field_image:destination_file','ifile');
        // $this->addFieldMapping('field_image:source_dir')
        //    ->defaultValue('');

          $this->addFieldMapping('field_video_file:file_replace')->defaultValue(MigrateFile::FILE_EXISTS_REUSE);
        //$this->addFieldmapping('field_video_file:preserve_files')->defaultValue(TRUE);
        $this->addFieldMapping('field_video_file', 'video_file');
        $this->addFieldMapping('field_video_file:destination_file','vfile');
        $this->addFieldMapping('field_video_file:source_dir');
            //->defaultValue('http://origin.znn.india.com/hindi/');
        module_load_include('inc','pathauto','pathauto');

    }

    public function prepareRow($row) {
        if ($row->isYoutube == 1)
        {
            $row->vfilename = null;
        }
        else
        {
            $row->youtubecode = null;

            $vFiled = pathinfo($row->vfile);
            $row->vfile = pathauto_cleanstring(strtolower($vFiled['filename'])) .'.'.$vFiled['extension'];
        }

        if (strpos($row->video_file,'http') !== false)
        {
            //print $row->img_path;
        }
        else
        {
            $row->video_file = 'http://origin.znn.india.com/hindi/' . $row->video_file;

        }

        if (strpos($row->video_image,'http') !== false)
        {
            //print $row->img_path;
        }
        else
        {
            $row->video_image = 'http://origin.znn.india.com/hindi/' . $row->video_image;

        }


        if (strlen($row->ifile) > 4)
        {
            $vFiled = pathinfo($row->ifile);
            $row->ifile = pathauto_cleanstring(strtolower($vFiled['filename'])) .'.'.$vFiled['extension'];

        }
        else{
            $row->video_image = null;
        }
        $row->video_title =  ((strip_tags(trim($row->video_title))));
        $row->video_desc = ((strip_tags(trim($row->video_desc))));



        $row->url='news/video' .$row->url;
    }
    public function prepare($node, stdClass $row) {
        // $node->nid=1505000 + $node->nid;

        // Let's pretend we couldn't have done this as easily in prepareRow...
        //  $node->path = 'news/' . $row->znm_news_id . '.html';
    }

}




/**
 * The Zee News Hindi Gallery Migration
 */
class ZNHGallery extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');
//        $this->dependencies = array('ZeeNewsCategory');
        $options = array('track_changes' => 1);
        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'photo_id' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'znews',
                    'description' => 'catid from legacy table category',
                )
            ),
            MigrateDestinationNode::getKeySchema()
        );

        $query = Database::getConnection('zeehindi161', 'for_migration')
            ->select('news_gallery', 'znews');
        $query->fields('znews', array('photo_id','photo_title','url','photo_date','photo_image','ifile'));
           $query->condition('photo_id', '140300', '>=');

        $query->orderBy('photo_id', 'asc');//$query->range(0, 5);

        //  $query->addExpression('GROUP_CONCAT(DISTINCT section)', 'section');

        // Create a MigrateSource object, which manages retrieving the input data.
        $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationNode('photo_gallery', array('text_format' => 'full_html'));
        // $this->addFieldMapping('nid', 'secid');
        $this->addFieldMapping('field_oldid', 'photo_id');
        $this->addFieldMapping('title', 'photo_title');
        $this->addFieldMapping('field_home_title', 'photo_title');
        $this->addFieldMapping('is_new')->defaultValue(TRUE);
        $this->addFieldMapping('uid')->defaultValue(1);
        $this->addFieldMapping('language')->defaultValue('en');
        $this->addFieldMapping('created', 'photo_date');
        $this->addFieldMapping('changed', 'photo_date');
        $this->addFieldMapping('field_domain')->defaultValue("8");
        $this->addFieldMapping('pathauto')->defaultValue("0");
        $this->addFieldMapping('path','url');
        $this->addFieldMapping('field_image', 'photo_image');
        $this->addFieldMapping('field_image:destination_file','ifile');
        module_load_include('inc','pathauto','pathauto');

        //$this->addFieldMapping('path','znm_news_stitle');



        // We conditionally DNM these fields, so your field mappings will be clean
        // whether or not you have path and or pathauto enabled
//
//        if (module_exists('path')) {
//            $this->addFieldMapping('path')
//                ->issueGroup(t('DNM'));
//            if (module_exists('pathauto')) {
//                $this->addFieldMapping('pathauto')
//                    ->issueGroup(t('DNM'));
//            }
//        }
    }

    public function prepareRow($row) {
        if (strpos($row->photo_image,'http') !== false)
        {
            //print $row->img_path;
        }
        else
        {
            $row->photo_image = 'http://origin.znn.india.com/hindi' . $row->photo_image;

        }
        $vFiled = pathinfo($row->ifile);
        $row->ifile = pathauto_cleanstring(strtolower($vFiled['filename'])) .'.'.$vFiled['extension'];

        $row->url='news/images'.$row->url;
        //   $row->secid=1500000 + $row->secid;

        //   $row->znm_news_title= $row->znm_news_title .'Rana';

    }

}


/**
 * The NodeImagesMigration - Field Collection
 */
class ZNHGalleryImages extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        module_load_include('inc', 'field_collection', 'field_collection.migrate');
        $this->description = t('Migrate styles from the source database to taxonomy terms');
        $this->dependencies = array('ZNHGallery');
        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'image_id' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'znews',
                    'description' => 'catid from legacy table category',
                )
            ),
            MigrateDestinationFieldCollection::getKeySchema()
        );


        $query = Database::getConnection('zeehindi161', 'for_migration')
            ->select('news_images', 'znews');
        $query->fields('znews', array('image_id','image_title','image_file','photo_id','ifile'));
        //   $query->condition('secid', '811', '<');
       // $query->condition('photo_id', '153544', '<=');
        $query->condition('photo_id', '140300', '>=');
       // $query->condition('photo_id', '139300', '>=');

        $query->orderBy('image_id', 'asc');
//        $query->range(0, 200);

        //  $query->addExpression('GROUP_CONCAT(DISTINCT section)', 'section');

        // Create a MigrateSource object, which manages retrieving the input data.
        $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationFieldCollection('field_gallery_data',array('host_entity_type' => 'node'));

        $this->addFieldMapping('host_entity_id', 'photo_id')
            ->sourceMigration('ZNHGallery');
            $this->addFieldMapping('field_image:file_replace')->defaultValue(MigrateFile::FILE_EXISTS_REUSE);
        //    $this->addFieldmapping('field_image:preserve_files')->defaultValue(TRUE);
        $this->addFieldMapping('field_image', 'image_file');
        $this->addFieldMapping('field_image:destination_file','ifile');
        $this->addFieldMapping('field_caption', 'image_title');
        //   $this->addFieldMapping('pathauto')->defaultValue("0");
        //$this->addFieldMapping('field_caption')->defaultValue('Ultra Orthodox Jewish men wrapped in prayer shawls participate in the special "Blessing of the Sun" prayer at historic site of Masada overlooking the Dead Sea, in Masada, Israel, Wednesday. Devout Jews around the world on Wednesday observed a ritual perfo sdsds jsdhjsdhj 123');
        // ... Add field mapping here.

        module_load_include('inc','pathauto','pathauto');

        // We conditionally DNM these fields, so your field mappings will be clean
        // whether or not you have path and or pathauto enabled

    }

    public function prepareRow($row) {
        if (strpos($row->image_file,'http') !== false)
        {
            //print $row->img_path;
        }
        else
        {
            $row->image_file = 'http://origin.znn.india.com/hindi/' . $row->image_file;

        }
        $vFiled = pathinfo($row->ifile);
        $row->ifile = pathauto_cleanstring(strtolower($vFiled['filename'])) .'.'.$vFiled['extension'];
    }
}


/**
 * The Zee News Hindi Slideshow Migration
 */
class ZNHSlideshow extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        // Human-friendly description of your migration process. Be as detailed as you
        // like.

        $this->description = t('Migrate styles from the source database to taxonomy terms');
//        $this->dependencies = array('ZeeNewsCategory');
        $options = array('track_changes' => 1);
        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'sid' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'znews',
                    'description' => 'catid from legacy table category',
                )
            ),
            MigrateDestinationNode::getKeySchema()
        );

        $query = Database::getConnection('zeehindi161', 'for_migration')
            ->select('news_slides', 'znews');
        $query->fields('znews', array('sid','sec_name','sec_img','entry_date','url','ifile'));
        $query->orderBy('sid', 'asc');//$query->range(0, 5);

        //  $query->addExpression('GROUP_CONCAT(DISTINCT section)', 'section');

        // Create a MigrateSource object, which manages retrieving the input data.
        $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationNode('slideshow', array('text_format' => 'full_html'));
        // $this->addFieldMapping('nid', 'secid');
        $this->addFieldMapping('field_oldid', 'sid');
        $this->addFieldMapping('title', 'sec_name');
        $this->addFieldMapping('field_home_title', 'sec_name');
        $this->addFieldMapping('is_new')->defaultValue(TRUE);
        $this->addFieldMapping('uid')->defaultValue(1);
        $this->addFieldMapping('language')->defaultValue('en');
        $this->addFieldMapping('created', 'entry_date');
        $this->addFieldMapping('changed', 'entry_date');
        $this->addFieldMapping('field_domain')->defaultValue("8");
        $this->addFieldMapping('pathauto')->defaultValue("0");
        $this->addFieldMapping('path','url');
        $this->addFieldMapping('field_image', 'sec_img');
       // $this->addFieldMapping('field_image:file_replace')->defaultValue(MigrateFile::FILE_EXISTS_REUSE);
        $this->addFieldMapping('field_image:source_dir')
            ->defaultValue('http://origin.znn.india.com/hindi/');
        $this->addFieldMapping('field_image:destination_file','ifile');

        module_load_include('inc','pathauto','pathauto');
        //$this->addFieldMapping('path','znm_news_stitle');



        // We conditionally DNM these fields, so your field mappings will be clean
        // whether or not you have path and or pathauto enabled
//
//        if (module_exists('path')) {
//            $this->addFieldMapping('path')
//                ->issueGroup(t('DNM'));
//            if (module_exists('pathauto')) {
//                $this->addFieldMapping('pathauto')
//                    ->issueGroup(t('DNM'));
//            }
//        }
        module_load_include('inc','pathauto','pathauto');
    }

    public function prepareRow($row) {
        $vFiled = pathinfo($row->ifile);
        $row->ifile = pathauto_cleanstring(strtolower($vFiled['filename'])) .'.'.$vFiled['extension'];
        $row->url='slideshow/'. $row->url . '_'.$row->sid.'.html';
    }

}



/**
 * The NodeImagesMigration - Field Collection
 */
class ZNHSlideImages extends ZeeNewsMigration {
    public function __construct() {
        parent::__construct();
        module_load_include('inc', 'field_collection', 'field_collection.migrate');
        // Human-friendly description of your migration process. Be as detailed as you
        // like.
        $this->description = t('Migrate styles from the source database to taxonomy terms');
        $this->dependencies = array('ZNHSlideshow');
        $this->map = new MigrateSQLMap(
            $this->machineName,
            array(
                'nid' => array(
                    'type' => 'int',
                    'not null' => TRUE,
                    'alias' => 'znews',
                    'description' => 'catid from legacy table category',
                )
            ),
            MigrateDestinationFieldCollection::getKeySchema()
        );

        $query = "SELECT nid,sid,story_image,story_title,story_desc,ifile FROM news_slideimages";
        $count_query = "SELECT COUNT(*) FROM news_slideimages";
        $fields = array(
            'nid' => t('story_id'),
            'sid' => t('story_title ID'),
            'story_image' => t('Story Image'),
            'story_title' => t('Story Title'),
            'story_desc' => t('story desc'),
            'ifile' => t('Image File '),
        );
        $this->source = new MigrateSourceMSSQL(array(
            'servername' => '180.179.93.214',
            'username' => 'sa',
            'password' => 'P@ssw0rd',
            'database' => 'ZeeNews_Hindi',
        ), $query, $count_query, $fields);


         // Create a MigrateSource object, which manages retrieving the input data.
    //    $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));

        // Set up our destination - terms in the migrate_example_beer_styles vocabulary
        $this->destination = new MigrateDestinationFieldCollection('field_gallery_data',array('host_entity_type' => 'node'));

        $this->addFieldMapping('host_entity_id', 'sid')
            ->sourceMigration('ZNHSlideshow');
       // $this->addFieldMapping('field_image:file_replace')->defaultValue(MigrateFile::FILE_EXISTS_REUSE);
        //    $this->addFieldmapping('field_image:preserve_files')->defaultValue(TRUE);
        $this->addFieldMapping('field_image', 'story_image');
        $this->addFieldMapping('field_caption', 'story_title');
        $this->addFieldMapping('field_image:source_dir')
            ->defaultValue('http://origin.znn.india.com/hindi/');
        $this->addFieldMapping('field_image:destination_file','ifile');
        module_load_include('inc','pathauto','pathauto');
        //   $this->addFieldMapping('pathauto')->defaultValue("0");
        //$this->addFieldMapping('field_caption')->defaultValue('Ultra Orthodox Jewish men wrapped in prayer shawls participate in the special "Blessing of the Sun" prayer at historic site of Masada overlooking the Dead Sea, in Masada, Israel, Wednesday. Devout Jews around the world on Wednesday observed a ritual perfo sdsds jsdhjsdhj 123');
        // ... Add field mapping here.


        // We conditionally DNM these fields, so your field mappings will be clean
        // whether or not you have path and or pathauto enabled

    }

    public function prepareRow($row) {
        $vFiled = pathinfo($row->ifile);
        $row->ifile = pathauto_cleanstring(strtolower($vFiled['filename'])) .'.'.$vFiled['extension'];
        $row->story_title = '<h3>'.$row->story_title .'</h3><br/>' . $row->story_desc  ;

    }
}

