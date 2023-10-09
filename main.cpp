#include <iostream>
#include <string>
#include <Windows.h>
#include <fstream>
using namespace std;

class Helper;
class Base;
class User;
class Page;
class Post;
class Facebook;
class Activity;
class Comment;
class Memory;

class Date
{
private:
	int day, month, year;
	
public:
	static Date CurrentDate;
	bool Compare();
	bool MemoryCheck();
	int YearsAgo();
	static void SetDate(int d, int m, int y);
	void ReadDate(ifstream & fin);
	void DisplayDate();
};

class Base
{
	protected:
		char* ID;
	public:
		Base();
		virtual ~Base();
		virtual void AddPostToTimeline(Post* post) = 0;
		virtual void PrintBase() = 0;
		virtual char* GetID() = 0;      
		virtual void ViewMemory() = 0;

};

class Page			:  public Base 
{
private:
	char* Title;
	Post** Timeline;
	int PostCount;

public:
	void LoadPage(ifstream&);
	Page();
	char* GetID();
	void DisplayPage();
	~Page();
	Post* GetLatestPost();
	void AddPostToTimeline(Post* post);
	void PrintBase();
	void ViewTimeline();
	void ViewMemory();
};

class User		: public Base
{
private:
	char* FirstName;
	char* LastName;
	User** FriendList;
	Page** LikedPages;
	Post** Timeline;
    int TotalFriends;
	int TotalPages;       //liked pages of a user
	int PostCount;
public:
	User();
	void LoadUser(ifstream& );
	char* GetID();
	char* GetFName();
	char* GetLName();
	void SetUserFriend(User*);
	void SetLikedPages(Page*);
	void DisplayUser();
	void DisplayFriends();
	void DisplayLikedPg();
	void ViewTimeline();
	void AddPostToTimeline(Post* post);
	void PrintBase();
	void ViewHome();
	Post* GetLatestPost();
	void ViewMemory();
	~User();
};

class Activity
{
private:
	int type;						//what the user is doing
	char* Value;					

public:
	Activity();
	Activity(Activity* activity);
	void LoadActivity(ifstream& fin);
	void PrintActivity();
	~Activity();
};

class Post			
{
protected:
	char* PostID;
	Base* PostedBy;
	int Type;
	Activity* activity;
	Date SharedDate;
	char* Text;
private:
	Base** LikedBy;
	Base** Commentor;
	Comment** comments;
	int TotalLikes ;
	int TotalComments;
public:
	Post();
	Post(char* text, Base* Sharer);
	void LoadPost(ifstream& fin);
	virtual void SetPostedBy(Base* PostBy);
	void SetLikedBy(Base* LikedBy);
	virtual void PrintPost();
	void SetPostID(char* Id);
	void SetPostedOn(Date date);
	Date GetPostedOn();
	void AddComment(Comment* Comment);
	void AddNewComment(char* comment, char* commentID, Base* Ptr);
	void ViewLikes();
	char* GetPostID();
	char* GetPostText();
	int GetType();
	Activity* GetActivity();
	Base* GetPostedBy();
	virtual ~Post();
	static int TotalPostCount;
};

int Post:: TotalPostCount= 0;
class Memory: public Post
{
private:
	Post* MemoryPost;
	char* NewText;
	
public:
	Memory();
	Memory(Post* ptr, char* text, Base* SharedBy);
	void PrintPost();
	~Memory();
};

class Comment
{
private:
	char* ID;
	Base* CommentedBy;
	char* text;

public:
	Comment();
	void SetCommentedBy(Base* Commentor);
	void SetID(char* id);
	void SetValue(char* txt);
	void DisplayComment();
    ~Comment();
};


class Facebook
{
private:
	User** UsersList;
	Page** PageList;
	Post** Posts;
	Comment** AllComments;
	int totalUsers;
	int totalPages;
	int totalPosts;
	int totalComments;
	
public:
	Facebook();
	void LoadData(char***& TempFriendList, char***& TempLikedPages);
	void LoadPosts();
	void LoadAllComments();
	void AssociateFriends(char***& TempFriendList);
	void AssociatePages(char***& TempLikedPages);
	Page* SearchPageByID(char* str);
	User* SearchUserByID(char* str);
	Base* SearchBaseByID(char* str);
	Post* SearchPostByID(char* str);
	void Run();
	void ViewLikedList(char* PostID);
	void Like(char* PostID, Base* CurrentUser);
	void AddNewComment(char* PostID, char* comment, Base* CurrentUser);
	void ViewPost(char* PostID);
	void SeeYourMemory(Base* ptr);
	void ShareMemory(char* PostID, char* Text, Base* sharer);
	~Facebook();
};


class Helper
{
	public:
		static char* GetStringFromBuffer(char* str);
		static int StringLenght(char*& str);
		static void StringCopy(char*& dest, char* src);
		static bool StringCompare(char* str1, char* str2);
		static char* GetBufferSpace(ifstream& fin);

};

int Helper:: StringLenght(char*& str )
{
	int lenght = 0;
	if(str!=NULL)
	{
		while(str[lenght] != '\0')
			 lenght++;
	}

	return lenght;
}

//deep copy of a string
void Helper::StringCopy(char*& dest, char* src)
{
	int len = StringLenght(src);
	if(dest==NULL)
	{
		dest = new char[len + 1];
		for(int i = 0; i < len; i++)
			dest[i] = src[i];
		dest[len] = '\0';
	}

	else if(dest != src)
	{
		if(StringLenght(dest) != len)
		{
			delete[] dest;
			dest = new char[len + 1];
		}

		for(int i = 0; i < len; i++)
			dest[i] = src[i];
		dest[len] = '\0';
	}
}

char* Helper::GetStringFromBuffer(char* str)
{
	int len = StringLenght(str);

	if(len != 0)
	{
		char* newStr = new char[len + 1];
		StringCopy(newStr, str);
	
		return newStr;
	}

	return NULL;
}

char* Helper::GetBufferSpace(ifstream& fin)
{
	char temp[20];
	int i = 0 ;
	for( ; fin.peek()!= '\t'; i++)
		fin >> temp[i];							

	char* NewStr = new char[i+1];

	for(int j = 0 ; j < i ; j++)
		NewStr[j] = temp[j];
	NewStr[i] = '\0';

	return NewStr;
}

//returns true if strings are equal and false otherwise
bool Helper::StringCompare(char* str1, char* str2)
{
	int i = 0;
	if(StringLenght(str1) != StringLenght(str2))
		return false;

	while(str1[i] != '\0')
	{
		if(str1[i] != str2[i])
		   return false;
		i++;
	}
	
	return true;
}

//------------------------------------------------------------------------------------------------
//date
Date Date::CurrentDate;

void Date::SetDate(int d , int m , int y)
{
	CurrentDate.day = d;
	CurrentDate.month = m;
	CurrentDate.year = y;
}


void Date::ReadDate(ifstream& fin)
{
	fin >> day >> month >> year;
}

//to check current date
bool Date::Compare()
{
	if(year == CurrentDate.year)
	{
		if(month == CurrentDate.month)
		{
			if(day == CurrentDate.day - 1 || day == CurrentDate.day)
				return true;
		}
	}

	return false;
}

bool Date::MemoryCheck()
{
	if(month == CurrentDate.month && day == CurrentDate.day && year < CurrentDate.year)
	   return true;
	return false;
}

int Date::YearsAgo()
{
	if(MemoryCheck())
		return CurrentDate.year - year; 
}

void Date::DisplayDate()
{
	cout << "\t\t\t\t" << day << "-" << month << "-" << year << endl;
}

//------------------------------------------------------------------------------------

Base::Base()
{
	ID = NULL;
}

Base::~Base()
{
	if(ID)
	   delete[] ID;
}


//-----------------------------------------------------------------------------------------------------
//page

Page::Page()
{
	Title = NULL;
	Timeline = new Post* [10];
	for(int i = 0 ; i < 10 ; i++)
		Timeline[i] = NULL;
	PostCount = 0;
}

//reads ID and title in temp array and stores them in char*

void Page::LoadPage(ifstream& fin)
{
	char temp[100];
	fin >> temp;
	ID = Helper::GetStringFromBuffer(temp);
	fin.ignore(); 
	fin.getline(temp, 80, '\n');
	Title = Helper::GetStringFromBuffer(temp);

}

char* Page::GetID()
{
	return ID;
}

void Page::DisplayPage()
{
	cout << ID << "-" << Title << endl;
}

void Page::AddPostToTimeline(Post* post)
{
	Timeline[PostCount++] = post;
}

void Page::PrintBase()
{
	cout << Title << " ";
}

Post* Page:: GetLatestPost()
{
	for(int i = 0 ; i < PostCount ; i++)
	{
		if(Timeline[i]->GetPostedOn().Compare())
			return Timeline[i];
	}
	return NULL;
}

void Page::ViewMemory()
{
	for(int i = 0; i < PostCount ; i++)
	{
		if(Timeline[i]->GetPostedOn().MemoryCheck())
		{
			cout << "~~~~~ We hope you enjoy looking back and sharing your memories on Facebook" 
				 << "from the most recent to those long ago. ~~~~~\n\n";

			cout << "On this Day\n";
			cout << Timeline[i]->GetPostedOn().YearsAgo() << " Years Ago\n";
			Timeline[i]->PrintPost();
		}
	}
}

void Page::ViewTimeline()
{
	for(int i = PostCount - 1; i >= 0 ; i--)							//to print latest posts at the top of timeline
		Timeline[i]->PrintPost();
	cout << endl;
}

Page::~Page()
{
	if(Title)
	   delete[] Title;

	for(int i = 0 ; i < PostCount ; i++)
		delete Timeline[i];

	if(Timeline)
		delete[] Timeline;
	Timeline = NULL;
}
//-----------------------------------------------------------------------------
//user

User::User()
{
	ID = FirstName = LastName = NULL;
	FriendList = new User*[10];
	for(int i = 0 ; i < 10 ; i++)
		FriendList[i] = NULL;

	LikedPages = new Page*[10];
	for(int i = 0 ; i < 10 ; i++)
		LikedPages[i] = NULL;

	Timeline = new Post*[10];
	for(int i = 0 ; i < 10 ; i++)
		Timeline[i] = NULL;

	TotalFriends = 0;
	TotalPages = 0;
	PostCount = 0;
}

//reads ID and name of user in temp and stores in dynamic arrays

void User::LoadUser(ifstream& fin)
{
	char temp[80];
	fin >> temp;
	ID = Helper::GetStringFromBuffer(temp);
	fin >> temp;
	FirstName = Helper::GetStringFromBuffer(temp);
	fin >> temp;
	LastName = Helper::GetStringFromBuffer(temp);
}

char* User::GetID()
{
	return ID;
}

char* User::GetFName()
{
	return FirstName;
}

char* User::GetLName()
{
	return LastName;
}

//Adds each user friend in their friendlist

void User:: SetUserFriend(User* Person)
{
	FriendList[TotalFriends] = Person;
	TotalFriends++;
}

//Adds liked page of each user in the list in their pagelist

void User::SetLikedPages(Page* Pg)
{
	LikedPages[TotalPages] = Pg;
	TotalPages++;
}

void User::DisplayUser()
{
	cout << ID << "--" << FirstName << " " << LastName << endl;
}

void User::DisplayFriends()
{
	for(int i = 0 ; i < TotalFriends ; i++)
		FriendList[i]->DisplayUser();
}

void User::DisplayLikedPg()
{
	for(int i = 0 ; i < TotalPages ; i++)
		LikedPages[i]->DisplayPage();
}

void User::AddPostToTimeline(Post* post)
{
	Timeline[PostCount] = post;
	PostCount++;
}

void User::ViewTimeline()
{
	for(int i = PostCount - 1; i >= 0 ; i--)							//to print latest posts at the top of timeline
		Timeline[i]->PrintPost();
	cout << endl;
}

Post* User::GetLatestPost()
{
	for(int i = PostCount - 1; i >= 0 ; i--)
	{
		if(Timeline[i]->GetPostedOn().Compare())
			return Timeline[i];
	}
	return NULL;
}

void User::ViewHome()
{
	
	for(int i = 0; i < TotalFriends ; i++)
	{
		if(FriendList[i]->GetLatestPost())
		  FriendList[i]->GetLatestPost()->PrintPost();
	}
	for(int i = 0; i < TotalPages ; i++)
	{
		if(LikedPages[i]->GetLatestPost())
		   LikedPages[i]->GetLatestPost()->PrintPost();
	}

}

void User::PrintBase()
{
	cout << FirstName << " " << LastName << " " ;
}

void User::ViewMemory()
{
	for(int i = 0; i < PostCount ; i++)
	{
		if(Timeline[i]->GetPostedOn().MemoryCheck())
		{
			cout << "~~~~~ We hope you enjoy looking back and sharing your memories on Facebook" 
				 << " from the most recent to those long ago. ~~~~~\n\n";
			cout << "\t\t\tOn this Day\n";
			cout << "\t\t\t" << Timeline[i]->GetPostedOn().YearsAgo() << " Years Ago\n\n";
			Timeline[i]->PrintPost();
		}
	}

}

User::~User()
{	
	if(FirstName)
		delete[] FirstName;

	if(LastName)
		delete[] LastName;

	for(int i = 0 ; i < PostCount ; i++)
		delete Timeline[i];
	
	if(Timeline)
		delete[] Timeline;
	
	if(FriendList)
		delete[] FriendList;

	if(LikedPages)
		delete[] LikedPages;

}

//-----------------------------------------------------------------------
//activity

Activity::Activity()
{
	Value = NULL;
}

//copy constructor for memory share

Activity::Activity(Activity* activity)		//copy constructor
{
	type = activity->type;
	Value = NULL;
	Helper::StringCopy(Value,activity->Value);
}

void Activity::LoadActivity(ifstream& fin)
{
	fin >> type;
	char* temp = new char[100];
	fin.getline(temp,100,'\n');
	Value = Helper::GetStringFromBuffer(temp);
}

void Activity::PrintActivity()
{
	cout << " is " ;
	if(type == 1)
	   cout << " feeling ";
	if(type == 2)
	   cout << " thinking about ";
	if(type == 3)
	   cout << " making ";
	if(type == 4)
	   cout << " celebrating ";
	cout << Value << endl;
}

Activity::~Activity()
{
	if(Value)
		delete[] Value;
}

//-------------------------------------------------------------------------------------
//post functions

Post::Post()
{
	PostID = NULL;
	PostedBy = NULL;
	Text = NULL;
	activity = NULL;
	LikedBy = new Base*[10];
	comments = new Comment*[10];
	Commentor = new Base* [10];
	TotalLikes = TotalComments = 0;
	TotalPostCount++;
	for(int i = 0; i < 10 ; i++)
	{
		LikedBy[i] = NULL;
	}

	for(int i = 0; i < 10 ; i++)
	{
		comments[i] = NULL;
	}

	for(int i = 0; i < 10 ; i++)
	{
		Commentor[i] = NULL;
	}
}

//overloaded  constructor

Post::Post(char* text, Base* sharer)
{
	
	SetPostedBy(sharer);
	Text = NULL;
	Helper::StringCopy(Text, text);
	activity = NULL;
	SetPostedOn(Date::CurrentDate);

	//for initializer
	LikedBy = new Base*[10];
	comments = new Comment*[10];
	Commentor = new Base* [10];
	TotalLikes = TotalComments = 0;

	for(int i = 0; i < 10 ; i++)
	{
		LikedBy[i] = NULL;
	}

	for(int i = 0; i < 10 ; i++)
	{
		comments[i] = NULL;
	}

	for(int i = 0; i < 10 ; i++)
	{
		Commentor[i] = NULL;
	}

	
}

void Post::LoadPost(ifstream& fin)
{
	fin.ignore();
	fin >> Type;
	char temp[150];
	fin >> temp;
	PostID = Helper::GetStringFromBuffer(temp);
	SharedDate.ReadDate(fin);
	SetPostedOn(SharedDate);
	fin.ignore();
	fin.getline(temp, 100, '\n');
	Text = Helper::GetStringFromBuffer(temp);
	if(Type == 2)
	{
		activity = new Activity;
		activity->LoadActivity(fin);
	}
	
}

void Post::SetPostedBy(Base* PostBy)
{
	PostedBy = PostBy;
}

void Post::SetLikedBy(Base* Liked)
{
	LikedBy[TotalLikes] = Liked;
	TotalLikes++;
}

void Post::SetPostedOn(Date date)
{
	SharedDate = date;
}

void Post::SetPostID(char* id)
{
	PostID = id;
}

char* Post::GetPostID()
{
	return PostID;
}

int Post::GetType()
{
	return Type;
}

void Post::PrintPost()
{
	if(Type == 2)
	{
		PostedBy->PrintBase();
		activity->PrintActivity();
	}
	else
	{
		PostedBy->PrintBase();
		cout << " shared ";
	}
	cout << Text << endl << endl;
	SharedDate.DisplayDate();
	cout << endl;
	if(TotalComments != 0)
		cout << "--Comments--\n";

	for(int i = 0 ; i < TotalComments ; i++)
	{
		comments[i]->DisplayComment();
		cout << endl;
	}
}

//to add comment from file
void Post::AddComment(Comment* Comment)
{
	comments[TotalComments] = Comment;
	TotalComments++;
}

//to add comment from user

void Post::AddNewComment(char* text, char* commentID, Base* Ptr)
{
	comments[TotalComments] = new Comment;
	comments[TotalComments]->SetCommentedBy(Ptr);
	comments[TotalComments]->SetValue(text);
	comments[TotalComments]->SetID(commentID);
	TotalComments++;
}

Date Post::GetPostedOn()
{
	return SharedDate;
}

char* Post::GetPostText()
{
	return Text;
}

Base* Post::GetPostedBy()
{
	return PostedBy;
}

Activity* Post::GetActivity()
{
	if(activity)
		return activity;
}

void Post::ViewLikes()
{
	for(int i = 0 ; i < TotalLikes ; i++)
	{
		cout << LikedBy[i]->GetID() << ":\t";
		LikedBy[i]->PrintBase();
		cout << endl;
	}
}
Post::~Post()
{
	if(PostID)
	   delete[] PostID;
	if(Text)
	   delete[] Text;
	if(activity)
		delete activity;


	for(int i = 0 ; i < TotalComments ; i++)
	{
		delete comments[i];
	}

	if(comments)
		delete[] comments;
	comments = NULL;
	
}
//-----------------------------------------------------------------------

Comment::Comment()
{
	ID = NULL;
	CommentedBy = NULL;
	text = NULL;
}
	
void Comment::SetCommentedBy(Base* Commentor)
{
	CommentedBy = Commentor;
}

void Comment::SetID(char* id)
{
	ID = Helper::GetStringFromBuffer(id);
}

void Comment::SetValue(char* txt)
{
	text = Helper::GetStringFromBuffer(txt);
}

void Comment::DisplayComment()
{
	cout << "\n\t";
	CommentedBy->PrintBase();
	cout << "wrote:\"" << text << " \" " << endl;
}
    
Comment::~Comment()
{
	if(ID)
	   delete[] ID;

	if(text)
		delete[] text;
	CommentedBy = NULL;
}

//-----------------------------------------------------------------------

Memory::Memory()
{
	MemoryPost = NULL;

}

Memory::Memory(Post* PostPtr, char* Text, Base* SharedBy) : Post(Text, SharedBy)
{
	TotalPostCount++;
	char* newPostID = new char[7];
	newPostID[0] = 'p';
	newPostID[1] = 'o';
	newPostID[2] = 's';
	newPostID[3] = 't';

	int i = 4;
	int total = TotalPostCount;
	if(total > 9)
	{
		newPostID[i] = total/10 + '0';
		i++;
		newPostID[i] = total%10 + '0';
	}
	else
	newPostID[i] = total + '0';
	i++;
	newPostID[i] = '\0';

	SetPostID(newPostID);
	MemoryPost = PostPtr;
}


void Memory::PrintPost()
{
	SetConsoleTextAttribute(GetStdHandle STD_OUTPUT_HANDLE, 4);
	cout << "~~~"; 
	PostedBy->PrintBase() ;
	cout << " shared a memory ~~~\n\n"; 
	SharedDate.DisplayDate();
	cout << endl;
	cout << "\" " << Text << "\ " << endl;
	
	cout << endl;
	cout << "\t\t\t" << MemoryPost->GetPostedOn().YearsAgo() << " Years Ago\n\n";

	if(MemoryPost->GetType() == 2)
	{
		MemoryPost->GetPostedBy()->PrintBase();
		MemoryPost->GetActivity()->PrintActivity();
	}
	else
	{
		PostedBy->PrintBase();
		cout << " shared ";
	}
	cout << MemoryPost->GetPostText() << endl << endl;
	MemoryPost->GetPostedOn().DisplayDate();
	cout << endl;

	SetConsoleTextAttribute(GetStdHandle STD_OUTPUT_HANDLE, 7);
}

Memory::~Memory()
{
	MemoryPost = NULL;
}

//-----------------------------------------------------------------------

Facebook::Facebook()
{
	UsersList = NULL;
	PageList = NULL;
	AllComments = NULL;
	totalPages = totalUsers = totalPosts = totalComments = 0;
}

//loads pages and users and reads liked pages and friends in temp arrays

void Facebook::LoadData(char***& TempFriendList, char***& TempLikedPages)
{
	ifstream fin("Pages.txt");

	fin >> totalPages;
	PageList = new Page*[totalPages];

	for(int i = 0; i < totalPages ; i++)
	{
		PageList[i] = new Page;
		PageList[i]->LoadPage(fin);
	}
	fin.close();

	fin.open("Users.txt");
	fin >> totalUsers;
	UsersList = new User* [totalUsers];
	char temp[30];
	
	TempFriendList = new char**[totalUsers];
	TempLikedPages = new char**[totalUsers];								//to count friends of each person
	for(int i = 0; i < totalUsers ; i++)
	{
		UsersList[i] = new User;
		UsersList[i]->LoadUser(fin);
		TempFriendList[i] = new char*[10];
		
		for(int l = 0 ; l < 10 ; l++)
			TempFriendList[i][l] = NULL;

		int j = 0; 
		fin.ignore();
		while(fin.peek() != '-')
		{
			Helper::StringCopy(TempFriendList[i][j], Helper::GetBufferSpace(fin));
			j++;
			fin.ignore();
		}

		fin.ignore();
		fin.ignore(3, '\t');

		TempLikedPages[i] = new char*[10];
		
		for(int l = 0 ; l < 10 ; l++)
			TempLikedPages[i][l] = NULL;

		j = 0; 
		while(fin.peek() != '-')
		{
			 Helper::StringCopy(TempLikedPages[i][j], Helper::GetBufferSpace(fin));
			 j++;
			 fin.ignore();
		}

		fin.ignore(3, '\n');

	}

	AssociateFriends(TempFriendList);
	AssociatePages(TempLikedPages);
}

// searchs ID in user list and returns pointer to that user

User* Facebook::SearchUserByID(char* str)
{
	for(int i = 0 ; i < totalUsers ; i++)
	{
		if(Helper::StringCompare(UsersList[i]->GetID(), str))
			return UsersList[i];
	}

}

// searchs ID in user list and returns pointer to that user

Page* Facebook::SearchPageByID(char* str)
{
	for(int i = 0 ; i < totalPages ; i++)
	{
		if(Helper::StringCompare(PageList[i]->GetID(), str))
			return PageList[i];
	}
}

// associate friends in the user's friend list and deletes the temp id array

void Facebook::AssociateFriends(char***& TempFriendList)
{
	for(int i = 0 ; i < totalUsers ; i++)
	{
		int j = 0;
		while(TempFriendList[i][j] != NULL)
		{
		      UsersList[i]->SetUserFriend(SearchUserByID(TempFriendList[i][j]));
			  delete[] TempFriendList[i][j];
			  j++;
		}
		delete[] TempFriendList[i];
	}
	delete[] TempFriendList;
}

// associate Pages in the user's liked page list and deletes the temp id array
	
void Facebook::AssociatePages(char***& TempLikedPages)
{
	for(int i = 0 ; i < totalUsers ; i++)
	{
		int j = 0;
		while(TempLikedPages[i][j] != NULL)
		{
			UsersList[i]->SetLikedPages(SearchPageByID(TempLikedPages[i][j]));
			delete[] TempLikedPages[i][j];
			j++;
		}
		delete[] TempLikedPages[i];
	}
	delete[] TempLikedPages;

}

void Facebook::LoadPosts()
{
	ifstream fin("Post.txt");
	fin >> totalPosts;
	Posts = new Post* [totalPosts];
	int UserCount = 0;
	char temp [30];
	
	for(int i = 0 ; i < totalPosts ; i++)
	{
		Posts[i] = new Post;
		Posts[i]->LoadPost(fin);
		fin >> temp;
		Posts[i]->SetPostedBy(SearchBaseByID(temp));
		if(temp[0] == 'u')
		{
			SearchUserByID(temp)->AddPostToTimeline(Posts[i]);
		}
		else
		{
			SearchPageByID(temp)->AddPostToTimeline(Posts[i]);
		}
		fin.ignore();
		
		while(fin.peek() != '-')
		{
			char* temp2 = new char[4];
			Helper::StringCopy(temp2 , Helper::GetBufferSpace(fin)); //to read likes with tabs
			fin.ignore();						  //for tabs
			Posts[i]->SetLikedBy(SearchBaseByID(temp2));
			if(temp2)
				delete[] temp2;	
		}
		fin.ignore();	
	}
	
}

Base* Facebook::SearchBaseByID(char* temp)
{
	if(temp[0] == 'u')
	{
	   return  SearchUserByID(temp);
	}
		
	else
	{
		return SearchPageByID(temp);
	}
		
}

Post* Facebook::SearchPostByID(char* str)
{
	for(int i = 0 ; i < totalPosts ; i++)
	{
		if(Helper::StringCompare(Posts[i]->GetPostID(), str))
			return Posts[i];
	}
}

void Facebook::LoadAllComments()
{
	ifstream fin("Comment.txt");
	fin >> totalComments;
	AllComments = new Comment*[totalComments];
	for(int i = 0 ; i < totalComments ; i++)
	{
		AllComments[i] = new Comment;
		char temp[150]; 
		fin >> temp;
		AllComments[i]->SetID(temp);
		fin.ignore();
		char* temp2 = new char[10];
		temp2 = Helper::GetBufferSpace(fin);						//to read post id till space
		fin.ignore();
		fin >> temp;
		AllComments[i]->SetCommentedBy(SearchBaseByID(temp));
		fin.ignore();
		fin.getline(temp, 100, '\n');
		AllComments[i]->SetValue(temp);
		SearchPostByID(temp2)->AddComment(AllComments[i]);
		if(temp2)
		   delete[] temp2;
	}

}
Facebook::~Facebook()
{
	for(int i = 0 ; i < totalUsers ; i++)
		delete UsersList[i];

	if(UsersList)
		delete[] UsersList;
	UsersList = NULL;

	for(int i = 0 ; i < totalPages ; i++)
		delete PageList[i];

	if(PageList)
		delete[] PageList;
	PageList = NULL;

}

void Facebook::Like(char* PostID, Base* Ptr)
{
	SearchPostByID(PostID)->SetLikedBy(Ptr);
}

void Facebook::ViewLikedList(char* PostId)
{
	cout << "~~~Liked By~~~\n";
	SearchPostByID(PostId)->ViewLikes();
}

void Facebook::AddNewComment(char* PostID, char* Comment, Base* Ptr)
{
	char* CommentID = new char[5];
	CommentID[0] = 'c';
	int i = 1;
	int total = ++totalComments;
	if(total > 9)
	{
		CommentID[i] = total/10 + '0';
		i++;
		CommentID[i] = total%10 + '0';
	}
	else
	CommentID[i] = total + '0';
	i++;
	CommentID[i] = '\0';
	SearchPostByID(PostID)->AddNewComment(Comment, CommentID, Ptr);
	
	if(CommentID)
		delete[] CommentID;
}

void Facebook::ViewPost(char* PostID)
{
	SearchPostByID(PostID)->PrintPost();
}

void Facebook::ShareMemory(char* PostID, char* Text, Base* Sharer)
{
	Memory* NewMemory = new Memory(SearchPostByID(PostID), Text, Sharer);
	Sharer->AddPostToTimeline(NewMemory);		

}

void Facebook::Run()
{
	SetConsoleTextAttribute(GetStdHandle STD_OUTPUT_HANDLE, 1);
	cout << "=====================================================================================================" << endl;
	cout << "\n\n\t\t\t\tFACEBOOK \n\n";
	cout << "=====================================================================================================" << endl << endl;

	SetConsoleTextAttribute(GetStdHandle STD_OUTPUT_HANDLE, 7);
	cout << "------------------------------------------------------------------------------------------------------\n\n";
	cout << "Command:\tSet current user \"u7\" \n\n";
	cout << "------------------------------------------------------------------------------------------------------\n\n";
	User* CurrentUser = SearchUserByID("u7");
	cout << CurrentUser->GetFName() << " " << CurrentUser->GetLName() << " successfully set as Current User\n\n\n";
	
	cout << "------------------------------------------------------------------------------------------------------\n\n";
	cout << "Command :\t View Friend List\n\n";
	cout << "------------------------------------------------------------------------------------------------------\n\n";
	cout << CurrentUser->GetFName() << " " << CurrentUser->GetLName() << " -- Friend List\n\n\n";
	CurrentUser->DisplayFriends();
	cout << endl << endl;

	cout << "------------------------------------------------------------------------------------------------------\n\n";
	cout << "Command :\t View Liked Pages\n\n";
	cout << "------------------------------------------------------------------------------------------------------\n\n";
	cout << CurrentUser->GetFName() << " " << CurrentUser->GetLName() << " -- Liked Pages\n\n\n";
	CurrentUser->DisplayLikedPg();
	cout << endl << endl;
	
	cout << "------------------------------------------------------------------------------------------------------\n\n";
	cout << "Command :\t View Timeline\n\n";
	cout << "------------------------------------------------------------------------------------------------------\n\n";
	cout << CurrentUser->GetFName() << " " << CurrentUser->GetLName() << " -- Timeline\n\n\n";
	CurrentUser->ViewTimeline();

	cout << "------------------------------------------------------------------------------------------------------\n\n";
	cout << "Command :\t View Homepage\n\n";
	cout << "------------------------------------------------------------------------------------------------------\n\n";
	cout << CurrentUser->GetFName() << " " << CurrentUser->GetLName() << " -- Home\n\n\n";
	CurrentUser->ViewHome();
	cout << endl;

	cout << "------------------------------------------------------------------------------------------------------\n\n";
	cout << "Command :\t View Liked List (POST 5)\n\n";
	cout << "------------------------------------------------------------------------------------------------------\n\n";
	ViewLikedList("post5");

	cout << "------------------------------------------------------------------------------------------------------\n\n";
	cout << "Command :\t View Like post (POST 5)\n\n";
	cout << "------------------------------------------------------------------------------------------------------\n\n";
	Like("post5", CurrentUser);
	cout << "Current User Liked Post5\n\n";
	
	cout << "------------------------------------------------------------------------------------------------------\n\n";
	cout << "Command :\t View Liked List (POST 5)\n\n";
	cout << "------------------------------------------------------------------------------------------------------\n\n";
	ViewLikedList("post5");
	
	cout << "------------------------------------------------------------------------------------------------------\n\n";
	cout << "Command :\t Add comment on Post (POST 4)\n\n";
	cout << "------------------------------------------------------------------------------------------------------\n\n";
	AddNewComment("post4", "Good Luck for your exams! ", CurrentUser);
	ViewPost("post4");

	cout << "------------------------------------------------------------------------------------------------------\n\n";
	cout << "Command :\t  Vew Memory\n\n";
	cout << "------------------------------------------------------------------------------------------------------\n\n";
	CurrentUser->ViewMemory();

	cout << "------------------------------------------------------------------------------------------------------\n\n";
	cout << "Command :\t  Share Memory\n\n";
	cout << "------------------------------------------------------------------------------------------------------\n\n";
	ShareMemory("post10", "Never thought I will be specialist in this field..." , CurrentUser);

	cout << "------------------------------------------------------------------------------------------------------\n\n";
	cout << "Command :\t  View Timeline\n\n";
	cout << "------------------------------------------------------------------------------------------------------\n\n";
	CurrentUser->ViewTimeline();

	
	cout << "------------------------------------------------------------------------------------------------------\n\n";
	cout << "Command :\t  View Page (P1)\n\n";
	cout << "------------------------------------------------------------------------------------------------------\n\n";
	SearchPageByID("p1")->ViewTimeline();
	

	cout << "------------------------------------------------------------------------------------------------------\n\n";
	cout << "Command:\tSet current user \"u11\" \n\n";
	cout << "------------------------------------------------------------------------------------------------------\n\n";
	CurrentUser = SearchUserByID("u11");
	cout << CurrentUser->GetFName() << " " << CurrentUser->GetLName() << " successfully set as Current User\n\n\n";

	
	cout << "------------------------------------------------------------------------------------------------------\n\n";
	cout << "Command :\t View Timeline\n\n";
	cout << "------------------------------------------------------------------------------------------------------\n\n";
	cout << CurrentUser->GetFName() << " " << CurrentUser->GetLName() << " -- Timeline\n\n\n";
	CurrentUser->ViewTimeline();

	cout << "------------------------------------------------------------------------------------------------------\n\n";
	cout << "Command :\t View Homepage\n\n";
	cout << "------------------------------------------------------------------------------------------------------\n\n";
	cout << CurrentUser->GetFName() << " " << CurrentUser->GetLName() << " -- Home\n\n\n";
	CurrentUser->ViewHome();
	cout << endl;
	cout << " ------------------------------------ GOOD BYE :-) ----------------------------------------------------------\n";
}


void main()
{
	{
		char*** TempFriendList;
		char*** TempLikedPages;
        Facebook FB;
		FB.LoadData(TempFriendList, TempLikedPages);
		
		Date :: SetDate(15,11,2017);
		FB.LoadPosts();
		FB.LoadAllComments();
		FB.Run();
	}
		
	


	system("pause");
}