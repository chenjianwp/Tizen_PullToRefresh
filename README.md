Tizen_PullToRefresh
===================

PullToRefresh Library for Tizen!! 


This is Tizen Static Library Project. 

You just import 'libTizenPullToRefresh.a' file into lib folder

of your Tizen application Project. 

 (help link -> https://developer.tizen.org/ko/documentation/articles/working-static-libraries-tizen)


==============================================================================
<How to use Tizen PullToRefresh library>

->  Add PullToRefreshListView in form ProjectForm::OnInitializing(void) method!!


->  PullToRefreshListView* listview = new PullToRefreshListView();

  	listview->Construct(*this);
  	
  	listview->getRefreshableView()->SetItemProvider(*this);
  	
  	listview->setOnPullEventListener(*this);
  	
  	listview->setOnRefreshListener(*this);
  	
  	
  	
  	
